#include "gtest/gtest.h"

#include "Ash/MultiThread/FunctorSeq.h"

TEST(Ash, ThreadPoolTest)
{
	{
		std::atomic<bool> finished = false;
		std::mutex mu;
		std::condition_variable condi;
		
		std::vector<char> result( static_cast<size_t>(10) );
		std::atomic_int index = 0;
		//A->B[C|D]->E
		Ash::FunctorSeq::entry([&mu, &finished, &condi, &index, &result](Ash::FunctorSeq& seq)
		{
			int i = index++;
			result[i] = 'A';
			seq.then([&index, &result](Ash::FunctorSeq& seq)
			{
				int i = index++;
				result[i] = 'B';
				seq.then([&index, &result](Ash::FunctorSeq& seq)
				{
					int i = index++;
					result[i] = 'C';
					seq.then([&index, &result](Ash::FunctorSeq&)
					{
						int i = index++;
						result[i] = 'D';
					}, [&index, &result](Ash::FunctorSeq&)
					{
						int i = index++;
						result[i] = 'E';
					});
				});
			}, [&index, &result](Ash::FunctorSeq& seq)
			{
				int i = index++;
				result[i] = 'F';
			});
			seq.then([&index, &result](Ash::FunctorSeq& seq)
			{
				int i = index++;
				result[i] = 'G';
			});
			seq.then([&mu, &finished, &condi](Ash::FunctorSeq&)
			{
				std::unique_lock<std::mutex> lock(mu);
				finished = true;
				condi.notify_all();
			});
		});
		std::unique_lock<std::mutex> lock(mu);
		condi.wait(lock, [&finished]() {return finished.load(); });
		// A
		//   B
		//     C
		//       D E
		//   F
		// G
		EXPECT_TRUE(result[0] == 'A' && result[6] == 'G');
		auto itB = std::find(result.begin(), result.end(), 'B');
		auto itC = std::find(result.begin(), result.end(), 'C');
		auto itD = std::find(result.begin(), result.end(), 'D');
		auto itE = std::find(result.begin(), result.end(), 'E');
		auto itF = std::find(result.begin(), result.end(), 'F');
		EXPECT_TRUE(itC > itB && itD > itC && itE > itC);
	}

	std::vector<int> numbers(6000000);
	for (auto& i : numbers)
	{
		i = (rand() & 0xfff) << 12 | (rand() & 0xfff);
	}


	//MultiThread quick sort
	{
		bool useMR = true;
		const std::function<void(Ash::FunctorSeq&, int*, int, int)> quickSort = [&useMR, &quickSort](Ash::FunctorSeq& seq, int* arr, int begin, int end)
		{
			if (end - begin < 32)
			{
				std::sort(arr + begin, arr + end);
				return;
			}
			int mid = (end - begin) / 2;
			if (arr[begin] > arr[mid])std::swap(arr[begin], arr[mid]);
			if (arr[mid] > arr[end - 1])std::swap(arr[mid], arr[end - 1]);
			if (arr[begin] > arr[mid])std::swap(arr[begin], arr[mid]);
			std::swap(arr[begin], arr[mid]);
			int guard = arr[begin];
			int left = begin + 1;
			for (int i = begin + 1; i < end; ++i)
			{
				if (arr[i] < guard)
				{
					std::swap(arr[left], arr[i]);
					++left;
				}
			}
			std::swap(arr[begin], arr[left - 1]);
			if(useMR)
			{
				seq.then(
					std::bind(quickSort, std::placeholders::_1, arr, begin, left - 1),
					std::bind(quickSort, std::placeholders::_1, arr, left, end));
			}
			else
			{
				quickSort((Ash::FunctorSeq&)(*((Ash::FunctorSeq*)nullptr)), arr, begin, left - 1);
				quickSort((Ash::FunctorSeq&)(*((Ash::FunctorSeq*)nullptr)), arr, left, end);
			}
		};

		useMR = false;
		{
			std::vector<int> data = numbers;
			auto start = std::chrono::system_clock::now();
			quickSort((Ash::FunctorSeq&)(*((Ash::FunctorSeq*)nullptr)), data.data(), 0, (int)data.size());
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;
			DebugString("Sorted size=%ld random int array, No Multithread quick sort cost: %f seconds", data.size(), diff.count());
			bool orderIsRight = true;
			for (int i = 1; i < (int)data.size(); ++i)
			{
				if (data[i] < data[i - 1])
				{
					orderIsRight = false;
					break;
				}
			}
			EXPECT_TRUE(orderIsRight);
		}
		
		useMR = true;
		{
			std::vector<int> data = numbers;
			auto start = std::chrono::system_clock::now();
			{
				std::atomic<bool> finished = false;
				std::mutex mu;
				std::condition_variable condi;

				Ash::FunctorSeq::entry([&mu, &quickSort, &data, &finished, &condi](Ash::FunctorSeq& seq)
				{
					seq.then(std::bind(quickSort, std::placeholders::_1, data.data(), 0, (int)data.size()));
					seq.then([&mu, &finished, &condi](Ash::FunctorSeq&)
					{
						std::unique_lock<std::mutex> lock(mu);
						finished = true;
						condi.notify_all();
					});
				});

				std::unique_lock<std::mutex> lock(mu);
				condi.wait(lock, [&finished]() {return finished.load(); });
			}
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;
			DebugString("Sorted size=%ld random int array, MultiThread quick sort cost: %f seconds", data.size(), diff.count());
			bool orderIsRight = true;
			for (int i = 1; i < (int)data.size(); ++i)
			{
				if (data[i] < data[i - 1])
				{
					orderIsRight = false;
					break;
				}
			}
			EXPECT_TRUE(orderIsRight);
		}
	}

	// MultiThread merge sort
	if(1)
	{
		auto mergeArray = [](Ash::FunctorSeq&, int* arr, int size)
		{
			int mid = size / 2;
			if (mid == size || mid == 0 || size == 0)
				return;
			std::shared_ptr<int[]> mergedArrPtr{ new int[size] };
			int* mergedArr = mergedArrPtr.get();

			int left = 0;
			int right = mid;
			int curr = 0;
			while (left < mid && right < size)
			{
				if (arr[left] < arr[right])
					mergedArr[curr++] = arr[left++];
				else
					mergedArr[curr++] = arr[right++];
			}
			while (left < mid)
				mergedArr[curr++] = arr[left++];
			while (right < size)
				mergedArr[curr++] = arr[right++];
			memcpy(arr, mergedArr, size * sizeof(int));
		};
		std::function<void(Ash::FunctorSeq&, int*,int)> mergeSort = [&mergeSort, &mergeArray](Ash::FunctorSeq& seq, int* arr, int size)
		{
			if(size <= 32)
			{
				std::sort(arr, arr + size);	// small
			}
			else
			{
				int mid = size / 2;
				
				seq.then(std::bind(mergeSort, std::placeholders::_1, arr, mid), std::bind(mergeSort, std::placeholders::_1, arr + mid, size - mid))
					.then(std::bind(mergeArray, std::placeholders::_1, arr, size));
			}
		};
		//MultiThread merge sort
		{
			std::vector<int> data = numbers;
			auto start = std::chrono::system_clock::now();
			{
				std::atomic<bool> finished = false;
				std::mutex mu;
				std::condition_variable condi;
				
				Ash::FunctorSeq::entry([&mergeSort, &finished, &condi, &data](Ash::FunctorSeq& seq)
				{
					seq.then(std::bind(mergeSort, std::placeholders::_1, data.data(), (int)data.size()));
					seq.then([&finished, &condi](Ash::FunctorSeq&)
					{
						finished = true;
						condi.notify_all();
					});
				});
				std::unique_lock<std::mutex> lock(mu);
				condi.wait(lock, [&finished]() {return finished.load(); });
			}
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;
			DebugString("Sorted size=%ld random int array, MultiThread merge sort cost: %f seconds", data.size(), diff.count());
			bool orderIsRight = true;
			for (int i = 1; i < (int)data.size(); ++i)
			{
				if (data[i] < data[i - 1])
				{
					orderIsRight = false;
					break;
				}
			}
			EXPECT_TRUE(orderIsRight);
		}
	}

	////std::sort
	if(1)
	{
		auto start = std::chrono::system_clock::now();
		{
			std::sort(numbers.begin(), numbers.end(), std::less<>());
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		DebugString("Sorted size=%ld random int array, std::sort cost: %f seconds", numbers.size(), diff.count());
	}
}