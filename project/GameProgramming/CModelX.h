#ifndef CMODELX_H
#define CMODELX_H		

#include <vector>		//vectorクラスのインクルード(動的配列)
#include "CMatrix.h"	//マトリクスクラスのインクルード
#include "CVector.h"
#include "CMyShader.h"
class CModelX;		//CModelXクラスの宣言
class CSkinWeights;
class CMaterial;	//クラスの宣言
class CAnimationSet;
class CAnimation;
class CAnimationKey;

#define MODEL_FILE "resource\\ラグナ.x"	//入力ファイル名

//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//領域開放をマクロ化
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a;a=0;}

//CMeshクラスの定義
class CMesh{
public:
	int mVertexNum;	//頂点数
	CVector *mpVertex;	//頂点データ
	int mFaceNum;	//面数
	int *mpVertexIndex;	//面を構成する頂点番号
	int mNormalNum;		//法線数
	CVector *mpNormal;	//法線データ
	int mMaterialNum;	//マテリアル数
	int mMaterialIndexNum;	//マテリアル番号数(面数)
	int *mpMaterialIndex;	//マテリアル番号
	//テクスチャ座標データ
	float *mpTextureCoords;

	std::vector<CMaterial*>mMaterial;	//マテリアルデータ
	std::vector<CSkinWeights*>mSkinWeights;	//スキンウェイト

	//マテリアル毎の面数
	std::vector<int>mMaterialVertexCount;
	//頂点バッファ識別子
	GLuint mMyVertexBufferId;
	//頂点バッファ作成
	void CreateVertexBuffer();

	CVector *mpAnimateVertex;	//アニメーション用頂点
	CVector *mpAnimateNormal;	//アニメーション用法線

	//コンストラクタ
	CMesh()
		:mVertexNum(0)
		, mpVertex(0)
		, mFaceNum(0)
		, mpVertexIndex(nullptr)
		, mNormalNum(0)
		, mpNormal(nullptr)
		, mMaterialNum(0)
		, mMaterialIndexNum(0)
		, mpMaterialIndex(nullptr)
		, mpAnimateVertex(nullptr)
		, mpAnimateNormal(nullptr)
		, mpTextureCoords(nullptr)
		, mMyVertexBufferId(0)
	{}
	//デストラクタ
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		SAFE_DELETE_ARRAY(mpAnimateVertex);
		SAFE_DELETE_ARRAY(mpAnimateNormal);
		SAFE_DELETE_ARRAY(mpTextureCoords);
		//スキンウェイトの削除
		for (int i = 0; i < mSkinWeights.size(); i++){
			delete mSkinWeights[i];
		}
	}
	//読み込み処理
	void Init(CModelX *model);
	void Render();

	//頂点にアニメーション適用
	void AnimateVertex(CModelX *model);

	void AnimateVertex(CMatrix*);
};

//CModelXFrameクラスの定義
class CModelXFrame{
public:
	std::vector<CModelXFrame*>mChild;	//子フレームの配列
	CMatrix mTransformMatrix;	//変換行列
	char* mpName;	//フレーム名前
	int mIndex;		//フレーム番号
	CMesh mMesh;	//Meshデータ
	//コンストラクタ
	CModelXFrame(CModelX* model);
	CModelXFrame()
		:mpName(nullptr)
		, mIndex(0){}	//コンストラクタ
	//デストラクタ
	~CModelXFrame(){
		//子フレームをすべて解放する
		std::vector<CModelXFrame*>::iterator itr;
		for (itr = mChild.begin(); itr != mChild.end(); itr++){
			delete *itr;
		}
		//名前のエリアを解放する
		SAFE_DELETE_ARRAY(mpName);
	}
	void Render();
	CMatrix mCombinedMatrix;	//合成行列
	//合成行列の作成
	void AnimateCombined(CMatrix* parent);
};


/*
CModelX
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX
{
public:
	char* mpPointer;	//読み込み位置
	char mToken[1024];	//取り出した単語の領域
	std::vector<CModelXFrame*>mFrame;	//フレームの配列
	std::vector<CAnimationSet*>mAnimationSet;	//アニメーションセットの配列
	std::vector<CMaterial*>mMaterial;	//マテリアルの配列

	CModelX()
		:mpPointer(nullptr)
		, mpSkinningMatrix(nullptr)
	{}

	~CModelX(){
		if (mFrame.size() > 0){
			delete mFrame[0];
		}
		for (int i = 0; i < mAnimationSet.size(); i++){
			delete mAnimationSet[i];
		}
		for (int i = 0; i < mMaterial.size(); i++){
			delete mMaterial[i];
		}
		SAFE_DELETE_ARRAY(mpSkinningMatrix);
	}
	//ファイルの読み込み
	void Load(char* file);
	//単語の取り出し
	void GetToken();
	//ノードの読み飛ばし
	void SkipNode();
	//浮動小数点のデータの取得
	float GetFloatToken();
	//整数データの取得
	int GetIntToken();
	void Render();
	//フレーム名に該当するフレームのアドレスを返す
	CModelXFrame* FindFrame(char* name);
	void AnimateFrame();
	//スキンウェイトのフレーム番号設定
	void SetSkinWeightFrameIndex();
	//頂点にアニメーションを適用
	void AnimateVertex();
	//マテリアルの検索
	CMaterial* FindMaterial(char* name);

	void AnimateVertex(CMatrix*);
	/*
	アニメーションを抜き出す
	idx:分割したいアニメーションセットの番号
	start:分割したいアニメーションの開始時間
	end:分割したいアニメーションの終了時間
	name:追加するアニメーションセットの名前
	*/
	void SeparateAnimationSet(int idx, int start, int end, char* name);

	//シェーダー用スキンマトリックス
	CMatrix *mpSkinningMatrix;
	CMyShader mShader;	//シェーダーのインスタンス

	//シェーダーを使った描画
	void RenderShader(CMatrix *m);
};

/*
CSkinWeights
スキンウェイトクラス
*/
class CSkinWeights{
public:
	char *mpFrameName;	//フレーム名
	int mFrameIndex;	//フレーム番号
	int mIndexNum;		//頂点番号数
	int *mpIndex;		//頂点番号配列
	float *mpWeight;	//頂点ウェイト配列
	CMatrix mOffset;	//オフセットマトリックス

	CSkinWeights(CModelX *model);

	~CSkinWeights(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}
};
/*
CAnimationSet
アジメーションセット
*/
class CAnimationSet{
public:
	float mTime;		//現在時間
	float mWeight;		//重み
	float mMaxTime;		//最大時間
	//アニメーションセット名
	char *mpName;
	//アニメーション
	std::vector<CAnimation*>mAnimation;
	CAnimationSet(CModelX *model);
	CAnimationSet(){}
	~CAnimationSet(){
		SAFE_DELETE_ARRAY(mpName);
		//アニメーション要素の削除
		for (int i = 0; i < mAnimation.size(); i++){
			delete mAnimation[i];
		}
	}
};
/*
CAnimation
アニメーションクラス
*/
class CAnimation{
public:
	char *mpFrameName;		//フレーム名
	int mFrameIndex;		//フレーム番号

	int mKeyNum;	//キー数(時間数)
	CAnimationKey *mpKey;	//キーの配列

	CAnimation(CModelX *model);
	CAnimation(){}

	~CAnimation(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpKey);
	}
};
/*
CAnimationKey
アニメーションキークラス
*/
class CAnimationKey{
public:
	//時間
	float mTime;
	//行列
	CMatrix mMatrix;
};


#endif