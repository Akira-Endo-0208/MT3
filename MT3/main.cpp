#include "DxLib.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <cstring>

int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);
int DrawCircle(Vector2 vec, int r, unsigned int color);
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color);

int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,
	const Vector3& cameraTarget,
	const Vector3& cameraUp
);

int MV1SetMatrix(const int MHandle, const Matrix4 Matrix);

void DrawAxis3D(const float length);
void DrawKeyOperation();

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "LE2D_04_エンドウ_アキラ: MT3";

// ウィンドウ横幅
const int WIN_WIDTH = 1024;

// ウィンドウ縦幅
const int WIN_HEIGHT = 576;



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0x64);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み


	// ゲームループで使う変数の宣言



	Vector3 cameraPosition(0.0f, 200.0f, 0.0f);
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp(0.0f, 0.0f, 1.0f);


	SetCameraNearFar(1.0f, 1000.0f);
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
	SetCameraPositionAndTargetAndUpVec(
		cameraPosition,
		cameraTarget,
		cameraUp
	);

	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	Vector3 start(-100.0f, 0, 0);
	Vector3 end(+100.0f, 0, 0);
	//補間で使うデータ
	//start(p0) -> end(p2)を　5[s]　で完了させる

	//スタート地点
	Vector3 p0( -100.0f, 0.0f, 0.0f);	
	//制御点
	Vector3 p1(-10.0f, 0.0f, +100.0f);
	//制御点
	Vector3 p2( 10.0f, 0.0f, -100.0f);
	//エンド地点
	Vector3 p3( +100.0f, 0.0f, 0.0f);

	float maxTime = 5.0f;
	float timeRate;

	Vector3 position;

	startCount = GetNowHiPerformanceCount();

	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);

	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存

		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		if (CheckHitKey(KEY_INPUT_R))
		{
			startCount = GetNowHiPerformanceCount();
		}

		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float> (elapsedCount) / 1'000'000.0f;

		timeRate = min(elapsedTime / maxTime, 1.0f);

		Vector3 a = lerp(p0, p1, timeRate);
		Vector3 b = lerp(p1, p2, timeRate);
		Vector3 c = lerp(p2, p3, timeRate);

		Vector3 ab = lerp(a, b, timeRate);
		Vector3 bc = lerp(b, c, timeRate);
		position = lerp(ab, bc, timeRate);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//
		DrawAxis3D(500.0f);

		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);

		DrawSphere3D(p0, 3.0f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);

		DrawSphere3D(p1, 3.0f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);

		DrawSphere3D(p2, 3.0f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);

		DrawSphere3D(p3, 3.0f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);

		DrawFormatString(0, 0, GetColor(255, 255, 255), "position (%6.1f,%6.1f,%6.1f)", position.x, position.y, position.z);

		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f[s]", elapsedTime);

		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R] : Restart");

		DrawFormatString(0, 60, GetColor(255, 255, 255), "p0 : (%6.1f,%6.1f,%6.1f)",p0.x,p0.y,p0.z);

		DrawFormatString(0, 80, GetColor(255, 255, 255), "p1 : (%6.1f,%6.1f,%6.1f)", p1.x, p1.y, p1.z);

		DrawFormatString(0, 100, GetColor(255, 255, 255), "p2 : (%6.1f,%6.1f,%6.1f)", p2.x, p2.y, p2.z);

		DrawFormatString(0, 120, GetColor(255, 255, 255), "p3 : (%6.1f,%6.1f,%6.1f)", p3.x, p3.y, p3.z);
		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}

int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
	const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag) {
	VECTOR topPos = { TopPos.x,TopPos.y,TopPos.z };
	VECTOR bottomPos = { BottomPos.x,BottomPos.y,BottomPos.z };

	return DrawCone3D(topPos, bottomPos, r, DivNum, DifColor, SpcColor, FillFlag);

}

void DrawAxis3D(const float length)
{
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));

	/*const float coneSize = 10.0f;

	DrawCone3D(Vector3(length, 0, 0), Vector3(length - coneSize, 0, 0), coneSize / 2, 16,
		GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
	DrawCone3D(Vector3(0, length, 0), Vector3(0, length - coneSize, 0), coneSize / 2, 16,
		GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
	DrawCone3D(Vector3(0, 0, length), Vector3(0, 0, length - coneSize), coneSize / 2, 16,
		GetColor(0, 0, 255), GetColor(255, 255, 255), TRUE);*/
}

//オーバーロード関数
//表示位置にベクトル(Vector2)を指定できるようにする
int DrawCircle(Vector2 vec, int r, unsigned int color)
{
	return DrawCircle(static_cast<int>(vec.x), static_cast<int>(vec.y), r, color);
}

int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color)
{
	VECTOR p1 = { Pos1.x, Pos1.y, Pos1.z };
	VECTOR p2 = { Pos2.x, Pos2.y, Pos2.z };

	return DrawLine3D(p1, p2, Color);
}

int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
	VECTOR centerPos = { CenterPos.x,CenterPos.y,CenterPos.z };

	return DrawSphere3D(centerPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,
	const Vector3& cameraTarget,
	const Vector3& cameraUp
)
{
	VECTOR position = { cameraPosition.x,cameraPosition.y,cameraPosition.z };
	VECTOR target = { cameraTarget.x,cameraTarget.y,cameraTarget.z };
	VECTOR up = { cameraUp.x,cameraUp.y,cameraUp.z };

	return SetCameraPositionAndTargetAndUpVec(position, target, up);
}



void DrawKeyOperation()
{
	const unsigned white = GetColor(255, 255, 255);

	DrawFormatString(10, 20 * 1, white, " [W][E][R]   R : リセット");
	DrawFormatString(10, 20 * 2, white, " [A][S][D]  AD : y軸まわりの回転");
	DrawFormatString(10, 20 * 3, white, " [Z]        WS : x軸まわりの回転");
	DrawFormatString(10, 20 * 4, white, "            EZ : z軸まわりの回転");
}



int MV1SetMatrix(const int MHandle, const Matrix4 Matrix) {
	MATRIX matrix;
	std::memcpy(&matrix, &Matrix, sizeof MATRIX);

	return MV1SetMatrix(MHandle, matrix);
}