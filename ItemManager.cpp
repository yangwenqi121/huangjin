#include "ItemManager.h"
#include <wincodec.h>
//-------------------------------------------------
//	ItemManager.cpp.
//	10/26/2019.		created.	
//	10/26/2019.		modified.
//	by yangwenqi.
//-------------------------------------------------

game::ItemManager::ItemManager()
{

}
//-------------------------------------------------
game::ItemManager::~ItemManager()
{
}
//-------------------------------------------------
bool game::ItemManager::initialize(ID2D1HwndRenderTarget * renderTarget)
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
		L"ship.png",
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
		&_gold_bitmap);

	if (FAILED(hResult))
		return false;
	pConverter->Release();
	pDecoder->Release();
	_pWicImgFactory->Release();
	pSource->Release();

	/*initialzie items...*/
	for (auto &line : _items)
		for (auto& column : line)
			column = ItemType::IT_NONE; //将所有东西设置为空
	_items[0][3] = ItemType::IT_GOLD;
	_items[1][4] = ItemType::IT_GOLD;
	_items[2][7] = ItemType::IT_GOLD;

	return true;
}
//-------------------------------------------------
void game::ItemManager::render(ID2D1HwndRenderTarget * renderTarget)
{
	if (!renderTarget)
		return;
	for (int i = 0; i < 3; i++)
	{
		for (int j=0;j<10;++j)
		{
			if (_items[i][j]==ItemType::IT_GOLD)
			{
				D2D1_RECT_F rect;
				rect.left = j * 128;
				rect.right = rect.left + 128;
				rect.top = i * 128 + 250;
				rect.bottom = rect.top + 128;
				renderTarget->DrawBitmap(_gold_bitmap, &rect, 1.0f);
			}
		}
	}
}
void game::ItemManager::update(ID2D1HwndRenderTarget * renderTarget)
{
	//
}
ID2D1Bitmap * game::ItemManager::getItemBitmap(ItemType type)
{
	return _gold_bitmap;
}
game::ItemType game::ItemManager::isInsectWith(
	int x,
	int y)
{
	if (y < 250)
		return ItemType::IT_NONE;
	int index_x = x / 128; //碰到的是第几个的横坐标
	int index_y = (y - 250) / 128;//纵坐标
	if (index_x > 9 || index_y >2) //超过
		return ItemType::IT_NONE;
	if (_items[index_y][index_x] != ItemType::IT_NONE)
	{
		auto type= _items[index_y][index_x];
		_items[index_y][index_x] = ItemType::IT_NONE;
		return type;
	}
	return ItemType::IT_NONE;
}
//-------------------------------------------------