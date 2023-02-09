#include"Matrix.h"
#include"World.h"
#include <Novice.h>

const char kWindowTitle[] = "LC1A_06_カツヤマヨウヘイ_確認課題";


Vector2 Lerp(const Vector2& p0, const Vector2& p1, float t) {
	return { (1.0f - t) * p0.x + t * p1.x,(1.0f - t) * p0.y + t * p1.y };
}

Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
	Vector2 p0p1 = Lerp(p0, p1, t);
	Vector2 p1p2 = Lerp(p1, p2, t);
	return Lerp(p0p1, p1p2, t);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	//宣言
	const Vector2 kControlPoints[] = {
		{100,100},
		{400,400},
		{700,100},
	};

	Vector2 cameraPosition = { 480.0f,200.0f };
	Vector2 orthoLeftTop = { -640.0f,360.0f };
	Vector2 orthoRightTop = { 640.0f,-360.0f };
	Vector2 viewPortLeftTop = { 0.0f,0.0f };

	const Vector2 kAxisXStart = { -10000.0f,0.0f };
	const Vector2 kAxisXEnd = { 10000.0f,0.0f };
	const Vector2 kAxisYStart = { 0.0f,-10000.0f };
	const Vector2 kAxisYEnd = { 0.0f,10000.0f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix3x3 cameraMatrix = MakeAffineMatrix(Vector2{ 1.0f,1.0f }, 0.0f, cameraPosition);
		Matrix3x3 viewMatrix = Inverse(cameraMatrix);
		Matrix3x3 orthoMatrix = MakeOrthographicMatrix(orthoLeftTop.x, orthoLeftTop.y, orthoRightTop.x, orthoRightTop.y);
		Matrix3x3 viewPortMatrix = MakeViewPortMatrix(0, 0, 1280.0f, 720.0f);
		Matrix3x3 vpMatrix = Multiply(viewMatrix, orthoMatrix);
		vpMatrix = Multiply(vpMatrix, viewPortMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Vector2 axisXStart = Transform(kAxisXStart, vpMatrix);
		Vector2 axisXEnd = Transform(kAxisXEnd, vpMatrix);
		Vector2 axisYStart = Transform(kAxisYStart, vpMatrix);
		Vector2 axisYEnd = Transform(kAxisYEnd, vpMatrix);

		Novice::DrawLine(axisXStart.x, axisXStart.y, axisXEnd.x, axisXEnd.y, RED);
		Novice::DrawLine(axisYStart.x, axisYStart.y, axisYEnd.x, axisYEnd.y, RED);

		for (int controlIndex = 0; controlIndex < 3; ++controlIndex) {
			Vector2 controlPointScreen = Transform(kControlPoints[controlIndex], vpMatrix);
			Novice::DrawEllipse(controlPointScreen.x, controlPointScreen.y, 10, 10, 0.0f, WHITE, kFillModeSolid);
		}

		constexpr int kNumDevide = 32;
		constexpr float kNumDevideF = float(kNumDevide);

		for (int divideIndex = 0; divideIndex < kNumDevide; ++divideIndex) {
			float t = float(divideIndex) / kNumDevideF;
			float nextT = float(divideIndex + 1) / kNumDevideF;

			Vector2 bezierPoint = Bezier(kControlPoints[0], kControlPoints[1], kControlPoints[2], t);
			Vector2 bezierPointNext = Bezier(kControlPoints[0], kControlPoints[1], kControlPoints[2], t + 1.0f / 32.0f);

			Vector2 bezierPointScreen = Transform(bezierPoint, vpMatrix);
			Vector2 bezierPointNextScreen = Transform(bezierPointNext, vpMatrix);
			Novice::DrawLine(bezierPointScreen.x, bezierPointScreen.y, bezierPointNextScreen.x, bezierPointNextScreen.y, BLUE);
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
