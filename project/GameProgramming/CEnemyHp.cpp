#include "CEnemyHp.h"
#include "CEnemy2.h"

CMaterial CEnemyHp::sHpMaterial;

CEnemyHp::CEnemyHp(const CVector &pos, float w, float h, char *texture)
:CBillBoard(pos,w,h)
{
	if (sHpMaterial.mTexture.mId == 0){
		sHpMaterial.LoadTexture(texture);
		sHpMaterial.mDiffuse[0] = sHpMaterial.mDiffuse[1] =
			sHpMaterial.mDiffuse[2] = sHpMaterial.mDiffuse[3] = 1.0f;
	}
}

void CEnemyHp::Update(){
	CBillBoard::Update();
	mPosition = mPosition*CEnemy2::spThis->mMatrix;
}

void CEnemyHp::Render(){
	glDisable(GL_DEPTH_TEST);
	CBillBoard::Render(&sHpMaterial);
	glEnable(GL_DEPTH_TEST); //深度テスト有効
}