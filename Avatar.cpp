//-------------------------------------------------
//	Avatar.cpp.
//	10/12/2019.		created.	
//	10/16/2019.		modified.
//	by yangwenqi
//-------------------------------------------------
#include "Avatar.h"
#include <wincodec.h>
#include <cmath>
#include "game.h"
#include <limits>
#include "ItemManager.h"
#include <algorithm>
#include "ConcreteArguments.hpp"
//-------------------------------------------------
game::Listener_Remove::Listener_Remove(game::IListener * listener)
{
	_listener = listener;
}
//-------------------------------------------------
bool game::Listener_Remove::operator()(game::IListener * listener)
{
	return _listener == listener;

}

//-------------------------------------------------
game::Avatar::Avatar()
{
	//
}

//-------------------------------------------------
game::Avatar::~Avatar()
{
	//
}

//-------------------------------------------------
bool
game::Avatar::initialize(ID2D1HwndRenderTarget* renderTarget)
{
	/* creates format convector factory. */
	HRESULT hResult = ::CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(void**)&_pWicImgFactory);

	if (FAILED(hResult))
		return false;

	/* interfaces for image decoding... */
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pSource = nullptr;
	IWICFormatConverter* pConverter = nullptr;

	hResult = _pWicImgFactory->CreateDecoderFromFilename(
		L"Flyfish_Logo_Yellow.png",
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder);

	if (FAILED(hResult))
		return false;

	/* Create the initial frame. */
	hResult = pDecoder->GetFrame(0, &pSource);

	if (FAILED(hResult))
		return 0;

	hResult = _pWicImgFactory->CreateFormatConverter(&pConverter);

	if (FAILED(hResult))
		return 0;

	hResult = pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeMedianCut);

	if (FAILED(hResult))
		return false;

	/* creates a D2D bitmap from the WIC bitmap. */
	hResult = renderTarget->CreateBitmapFromWicBitmap(
		pConverter,
		nullptr,
		&_pBitmap);

	if (FAILED(hResult))
		return false;
	/*create brushes.*/
	hResult = renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(0.0f, 0.0f, 1.0f, 0.6f),
		&_pLineBrush);

	if (FAILED(hResult))
		return false;

	pConverter->Release();
	pDecoder->Release();
	_pWicImgFactory->Release();
	pSource->Release();

	return true;
}

//-------------------------------------------------
void
game::Avatar::update(ID2D1HwndRenderTarget* renderTarget)
{
	// get the geometry size of client screen.
	auto size = renderTarget->GetSize();

	// normal state.
	if (_avatar_state == AvatarState::AS_Normal)
	{
		normal_state(size);
	}
	// launched state.
	else
	{
		launched_state(size);
	}
}

//-------------------------------------------------
inline
void
game::Avatar::normal_state(D2D1_SIZE_F size)
{
	// check input.
	if (Game::getSingleton()->checkInput(INPUTBUTTON::IB_Space))
	{
		// change to the other state.
		_avatar_state = AvatarState::AS_Launched;
		return;
	}

	/* -> */
	if (_bOrientation)
	{
		_avatar_x += 2;
		if (_avatar_x + _pBitmap->GetSize().width / 2 >= size.width)
			_bOrientation = !_bOrientation;
	}
	/* <- */
	else
	{
		_avatar_x -= 2;
		if (_avatar_x <= 0)
			_bOrientation = !_bOrientation;
	}

	/* -> */
	if (_bAngleOrientation)
	{
		_fAngle += M_PI / 256.0f;
		if (_fAngle >= M_PI_2)
			_bAngleOrientation = !_bAngleOrientation;
	}
	/* <- */
	else
	{
		_fAngle -= M_PI / 256.0f;
		if (_fAngle <= -M_PI_2)
			_bAngleOrientation = !_bAngleOrientation;
	}

	/* finally, calculate parameters of line. */
	_fEndLineX = _fLineLength * std::sin(_fAngle) + _avatar_x + _pBitmap->GetSize().width / 4.0f;
	_fEndLineY = _fLineLength * std::cos(_fAngle) + _pBitmap->GetSize().height / 4.0f;
}

void game::Avatar::launched_state(D2D1_SIZE_F size)
{
	/*down */
	if (_bLineOrientation)
	{
		_extendLength += 2;
		if (_extendLength >=_MaxExtendLength)
		{
			_bLineOrientation = !_bLineOrientation;
			
		}
		if (Game::getSingleton()->item_mgr()->isInsectWith(_fEndLineX,_fEndLineY)==
			ItemType::IT_GOLD)
		{
			_bLineOrientation = !_bLineOrientation;
			_isComingBack = true;
		}


	}
	/*up*/
	else
	{
		_extendLength -= 2;
		if (_extendLength<=FLT_MIN)
		{
			if (_isComingBack)
			{
				_is_inCrease = true;
				notify(MessageType::MT_SCORE);
			}
			_bLineOrientation = true;
			_isComingBack = false;
			_avatar_state = AvatarState::AS_Normal;//切换状态
		}
	}
	/* finally, calculate parameters of line. */
	_fEndLineX = (_fLineLength+_extendLength) * std::sin(_fAngle) + _avatar_x + _pBitmap->GetSize().width / 4.0f;
	_fEndLineY = (_fLineLength +_extendLength)* std::cos(_fAngle) + _pBitmap->GetSize().height / 4.0f;
}


//-------------------------------------------------
void
game::Avatar::render(ID2D1HwndRenderTarget* renderTarget)
{
	// draw fish.
	D2D1_SIZE_F size = renderTarget->GetSize();
	D2D1_RECT_F rect;
	rect.left = _avatar_x;
	rect.top = 0.0f;
	rect.bottom = _pBitmap->GetSize().height / 2.0f;
	rect.right = _avatar_x+_pBitmap->GetSize().width / 2.0f;
	renderTarget->DrawBitmap(
		_pBitmap,
		&rect,
		1.0f);
	//draw gold
	
	if (_isComingBack)
	
		if (auto gold = Game::getSingleton()->item_mgr()->getItemBitmap(ItemType::IT_GOLD))
		{
			D2D1_RECT_F rect;
			rect.left = _fEndLineX-64;
			rect.top = _fEndLineY - 64;
			rect.bottom = rect.top + 128;
			rect.right = rect.left + 128;
			//画出来
			renderTarget->DrawBitmap(
				gold,
				&rect,
				1.0f);
		}
	
	// draw line.
	D2D1_POINT_2F pt1;
	D2D1_POINT_2F pt2;
	pt1.x = rect.left + _pBitmap->GetSize().width / 4.0f;
	pt1.y = rect.top + _pBitmap->GetSize().height / 4.0f;
	pt2.x = _fEndLineX;
	pt2.y = _fEndLineY;
	renderTarget->DrawLine(
		pt1,
		pt2,
		_pLineBrush,
		2.0f);
	
}

//-------------------------------------------------
void
game::Avatar::launch(D2D1_SIZE_F size)
{
	//
}

void game::Avatar::add_listener(MessageType mt, IListener * listener)
{
	if (!listener)
		return;
	_listeners[mt].emplace_back(listener);
}

void game::Avatar::remove_listener(MessageType mt, IListener * listener)
{
	if (!listener)
		return;
	auto it = _listeners.find(mt);
	if (it == _listeners.end())
		return;
	Listener_Remove lr(listener);
	//插入队列后边
	auto rm_it=std::remove_if(
		it->second.begin(),
		it->second.end(),
		lr);
	while (rm_it!=it->second.end())
	{
		rm_it =it->second.erase(rm_it);
	}
}

void game::Avatar::notify(MessageType mt)
{
	auto it = _listeners.find(mt);
	if (it == _listeners.end())
		return;
	
	std::shared_ptr<ScoreArg> sa(new ScoreArg);
	sa->_msg_type = MessageType::MT_SCORE;
	sa->_score_type = _is_inCrease ? ScoreType::ST_Increase : ScoreType::ST_Decrease;

	

	for (auto &item : it->second)
		item->on_update(mt, 
			std::static_pointer_cast<MsgArgument>(sa)
		);//最后一个人放弃他时，在堆上析构掉

}

//-------------------------------------------------

