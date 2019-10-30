//-------------------------------------------------
//	Game.cpp.
//	10/16/2019.		created.	
//	10/16/2019.		modified.
//	by yangwenqi
//-------------------------------------------------
#include "game.h"


//-------------------------------------------------
//	initialize static variables...
//-------------------------------------------------
game::Game* game::Game::_game = nullptr;

//-------------------------------------------------
game::Game*
game::Game::getSingleton(void)
{
	if (!_game)
		_game = new Game;

	return _game;
}

//-------------------------------------------------
void
game::Game::destroySingleton(void)
{
	if (_game)
	{
		delete _game;
		_game = nullptr;
	}
}

//-------------------------------------------------
bool
game::Game::initialize(HWND hWnd)
{
	long hResult = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&_direct2dFactory);

	if (FAILED(hResult))
		return false;

	RECT rect;
	GetClientRect(hWnd, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);
	D2D1_RENDER_TARGET_PROPERTIES properities = D2D1::RenderTargetProperties();
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_properties = D2D1::HwndRenderTargetProperties(hWnd, size);

	// creates a render target for drawing
	hResult = _direct2dFactory->CreateHwndRenderTarget(
		properities,
		hwnd_properties,
		&_renderTarget);

	if (FAILED(hResult))
		return false;

	_ticks = ::GetTickCount();

	// initialize avatar.
	if (_avatar)
		if (!_avatar->initialize(_renderTarget))
			return false;
	//initialize item manager
	if (_item_manager)
		if (!_item_manager->initialize(_renderTarget))
			return false;
	//initialize score manager
	_avatar->add_listener(MessageType::MT_SCORE,
		&*_score_mgr);
	return true;
}

//-------------------------------------------------
LRESULT
game::Game::processEvent(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	if (message == WM_KEYDOWN)
	{
		if (wParam == VK_SPACE)
			_inputs[INPUTBUTTON::IB_Space] = true;
	}
	else if (message == WM_SIZE)
	{
		if (_renderTarget)
		{
			D2D1_SIZE_U size;
			size.width = 0x0000ffff & lParam;
			size.height = 0x0000ffff & lParam >> 16;
			_renderTarget->Resize(size);
		}
	}
	return  DefWindowProc(hWnd, message, wParam, lParam);
}

//-------------------------------------------------
bool
game::Game::frame(void)
{
	unsigned long now = ::GetTickCount();
	if (now - _ticks < 10ul)
		return true;

	_ticks = now;

	// check inputs & update here...
	if (!_renderTarget)
		return false;
	//update stuff
	if (_avatar)
		_avatar->update(_renderTarget);

	_renderTarget->BeginDraw();
	_renderTarget->Clear(D2D1::ColorF(0.5f, 0.5f, 0.5f));
	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	// render stuffs here...
	//分层画背景
	if (_item_manager)
		_item_manager->render(_renderTarget);
	if (_avatar)
		_avatar->render(_renderTarget);

	
	

	_renderTarget->EndDraw();

	return true;
}
//-------------------------------------------------
bool game::Game::checkInput(INPUTBUTTON btn)
{
	auto it=_inputs.find(btn);
	if (it == _inputs.end()) //没找到
		return false;
	//找到了，检查值
	bool result = it->second; 
	it->second = false;
	return result;
	
}
//-------------------------------------------------
std::shared_ptr<game::ItemManager>
game::Game::item_mgr(void)
{
	return _item_manager;
}

//-------------------------------------------------
game::Game::Game()
{

	_avatar.reset(new Avatar);
	_item_manager.reset(new ItemManager);
	_score_mgr.reset(new ScoreMgr);
}

//-------------------------------------------------
game::Game::~Game()
{
	if (_renderTarget)
		_renderTarget->Release();
}

//-------------------------------------------------





