#pragma once

#include <streambuf>

// Use an array as stream target
class ArrayStream: public std::streambuf
{	
	enum
	{	// constants for bits in stream state
		_Constant = 2,	// set if character array nonmutable
		_Noread = 4,	// set if character array cannot be read
	};	// set if initial writes are appends
	typedef int _Strstate;
public:
	typedef std::streambuf _Mysb;
	typedef std::streambuf::traits_type _Traits;
	typedef char _Elem;

	ArrayStream()
	{
		_Tidy();
	}

	void initAsInputStream(const _Elem* data, size_t size)
	{
		_Tidy();
		_Init(const_cast<_Elem*>(data), size, _Constant);
	}
	void initAsOutputStream(_Elem* buffer, size_t size)
	{
		_Tidy();
		_Init(buffer, size, _Noread);
	}

	ArrayStream(const ArrayStream&) = delete;
	ArrayStream& operator=(const ArrayStream&) = delete;

	virtual ~ArrayStream()
	{	// destroy the object
		_Tidy();
	}


	typedef typename _Traits::int_type int_type;
	typedef typename _Traits::pos_type pos_type;
	typedef typename _Traits::off_type off_type;
protected:
	virtual int_type overflow(int_type _Meta = _Traits::eof())
	{	// put an element to stream
		if (_Mystate & _Constant)
			return (_Traits::eof());	// array nonmutable, fail
		else if (_Traits::eq_int_type(_Traits::eof(), _Meta))
			return (_Traits::not_eof(_Meta));	// EOF, return success code

		if (_Mysb::pptr() != 0
			&& _Mysb::pptr() < _Mysb::epptr())
		{	// room in buffer, store it
			*_Mysb::_Pninc() = _Traits::to_char_type(_Meta);
			return (_Meta);
		}
		else
		{
			return _Traits::eof();
		}
	}

	virtual int_type pbackfail(int_type _Meta = _Traits::eof())
	{	// put an element back to stream
		if (_Mysb::gptr() == 0
			|| _Mysb::gptr() <= _Mysb::eback()
			|| (!_Traits::eq_int_type(_Traits::eof(), _Meta)
				&& !_Traits::eq(_Traits::to_char_type(_Meta), _Mysb::gptr()[-1])
				&& _Mystate & _Constant))
			return (_Traits::eof());	// can't put back, fail
		else
		{	// back up one position and store put-back character
			_Mysb::gbump(-1);
			if (!_Traits::eq_int_type(_Traits::eof(), _Meta))
				*_Mysb::gptr() = _Traits::to_char_type(_Meta);
			return (_Traits::not_eof(_Meta));
		}
	}

	virtual int_type underflow() override
	{	// get an element from stream, but don't point past it
		if (_Mysb::gptr() == 0)
			return (_Traits::eof());	// no character buffer, fail
		else if (_Mysb::gptr() < _Mysb::egptr())
			return (_Traits::to_int_type(*_Mysb::gptr()));	// return buffered
		else if (_Mystate & _Noread || _Mysb::pptr() == 0
			|| (_Mysb::pptr() <= _Mysb::gptr() && _Seekhigh <= _Mysb::gptr()))
			return (_Traits::eof());	// can't read, fail
		else
		{	// extend read buffer into written area, then return buffered
			if (_Seekhigh < _Mysb::pptr())
				_Seekhigh = _Mysb::pptr();
			_Mysb::setg(_Mysb::eback(), _Mysb::gptr(), _Seekhigh);
			return (_Traits::to_int_type(*_Mysb::gptr()));
		}
	}

	virtual pos_type seekoff(off_type _Off,
	                         std::ios_base::seekdir _Way,
	                         std::ios_base::openmode _Which = std::ios_base::in | std::ios_base::out)
	{	// change position by _Off, according to _Way, _Mode
		if (_Mysb::pptr() != 0 && _Seekhigh < _Mysb::pptr())
			_Seekhigh = _Mysb::pptr();	// update high-water pointer

		if (_Which & std::ios_base::in && _Mysb::gptr() != 0)
		{	// position within read buffer
			if (_Way == std::ios_base::end)
				_Off += (off_type)(_Seekhigh - _Mysb::eback());
			else if (_Way == std::ios_base::cur
				&& (_Which & std::ios_base::out) == 0)
				_Off += (off_type)(_Mysb::gptr() - _Mysb::eback());
			else if (_Way != std::ios_base::beg)
				_Off = std::_BADOFF;

			if (0 <= _Off && _Off <= _Seekhigh - _Mysb::eback())
			{	// change read position
				_Mysb::gbump((int)(_Mysb::eback() - _Mysb::gptr() + _Off));
				if (_Which & std::ios_base::out && _Mysb::pptr() != 0)
					_Mysb::setp(_Mysb::pbase(), _Mysb::gptr(),
						_Mysb::epptr());	// change write position to match
			}
			else
				_Off = std::_BADOFF;
		}
		else if (_Which & std::ios_base::out && _Mysb::pptr() != 0)
		{	// position within write buffer
			if (_Way == std::ios_base::end)
				_Off += (off_type)(_Seekhigh - _Mysb::eback());
			else if (_Way == std::ios_base::cur)
				_Off += (off_type)(_Mysb::pptr() - _Mysb::eback());
			else if (_Way != std::ios_base::beg)
				_Off = std::_BADOFF;

			if (0 <= _Off && _Off <= _Seekhigh - _Mysb::eback())
				_Mysb::pbump((int)(_Mysb::eback()
					- _Mysb::pptr() + _Off));	// change write position
			else
				_Off = std::_BADOFF;
		}

		else if (_Off != 0)


			_Off = std::_BADOFF;	// neither read nor write buffer selected, fail
		return (pos_type(_Off));
	}

	virtual pos_type seekpos(pos_type _Ptr,
	                         std::ios_base::openmode _Mode = std::ios_base::in | std::ios_base::out)
	{	// change position to _Pos, according to _Mode
		std::streamoff _Off = (std::streamoff)_Ptr;
		if (_Mysb::pptr() != 0 && _Seekhigh < _Mysb::pptr())
			_Seekhigh = _Mysb::pptr();	// update high-water pointer

		if (_Off == std::_BADOFF)
			;
		else if (_Mode & std::ios_base::in && _Mysb::gptr() != 0)
		{	// position within read buffer
			if (0 <= _Off && _Off <= _Seekhigh - _Mysb::eback())
			{	// change read position
				_Mysb::gbump((int)(_Mysb::eback() - _Mysb::gptr() + _Off));
				if (_Mode & std::ios_base::out && _Mysb::pptr() != 0)
					_Mysb::setp(_Mysb::pbase(), _Mysb::gptr(),
						_Mysb::epptr());	// change write position to match
			}
			else
				_Off = std::_BADOFF;
		}
		else if (_Mode & std::ios_base::out && _Mysb::pptr() != 0)
		{	// position within write buffer
			if (0 <= _Off && _Off <= _Seekhigh - _Mysb::eback())
				_Mysb::pbump((int)(_Mysb::eback()
					- _Mysb::pptr() + _Off));	// change write position
			else
				_Off = std::_BADOFF;
		}
		else
			_Off = std::_BADOFF;	// neither read nor write buffer selected, fail
		return (std::streampos(_Off));
	}

	void _Init(_Elem *_Ptr,
		size_t _Count, _Strstate _State)
	{	// initialize buffer to [_Ptr, _Ptr + _Count), set state
		_Seekhigh = 0;
		_Mystate = _State;

		if (_Count != 0
			&& (_Mystate & (_Noread | _Constant)) != (_Noread | _Constant))
		{	// finite buffer that can be read or written, set it up
			//_Elem *_Pnew = _Unfancy(_Al.allocate(_Count));
			//_Traits::copy(_Pnew, _Ptr, _Count);
			_Elem *_Pnew = _Ptr;
			_Seekhigh = _Pnew + _Count;

			if (!(_Mystate & _Noread))
				_Mysb::setg(_Pnew, _Pnew,
					_Pnew + _Count);	// setup read buffer
			if (!(_Mystate & _Constant))
			{	// setup write buffer, and maybe read buffer
				_Mysb::setp(_Pnew,
					_Pnew,
					_Pnew + _Count);
				if (_Mysb::gptr() == 0)
					_Mysb::setg(_Pnew, 0, _Pnew);
			}
		}
	}

	void _Tidy()
	{
		_Mysb::setg(0, 0, 0);
		_Mysb::setp(0, 0);
		_Seekhigh = 0;
	}

private:
	_Strstate _Getstate(std::ios_base::openmode _Mode)
	{	// convert open mode to stream state bits
		_Strstate _State = (_Strstate)0;
		if (!(_Mode & std::ios_base::in))
			_State |= _Noread;
		if (!(_Mode & std::ios_base::out))
			_State |= _Constant;
		return (_State);
	}

	_Elem *_Seekhigh;	// the high-water pointer in character array
	_Strstate _Mystate;	// the stream state
};
