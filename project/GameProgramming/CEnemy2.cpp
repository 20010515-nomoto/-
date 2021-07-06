#include "CEnemy2.h"
#include "CEffect.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define OBJ "f16.obj"	//���f���̃t�@�C��
#define MTL "f16.mtl"	//���f���̃}�e���A���t�@�C��

#define HP 3	//�ϋv�l
#define VELOCITY 0.11f	//���x

CModel CEnemy2::mModel;	//���f���f�[�^�쐬

//#define FIRECOUNT 15	//���ˊԊu
CEnemy2 *CEnemy2::spThis = 0;

CEnemy2::CEnemy2()
: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
, mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 100.0f), 30.0f)
, mColAttackRange(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
, mColAttack(this, &mMatrix, CVector(0.0f, 0.0f, -2.0f), 2.0f)
, mpPlayer(0)
, mHp(HP)
, mFireCount(0)
, mAttackFlg(false)
, mAcquisitionFlg(false)
{
	//�^�O�ݒ�
	mTag = EENEMY;
	mColSearch.mTag = CCollider::ESEARCH;
	mColAttackRange.mTag = CCollider::EENEMY_ATTACK_RANGE;
	mColAttack.mTag = CCollider::EENEMY_ATTACK;		

	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	spThis = this;
}


//�R���X�g���N�^
//CEnemy(�ʒu, ��], �g�k)
CEnemy2::CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale)
	: CEnemy2()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;	//�ʒu�̐ݒ�
	mRotation = rotation;	//��]�̐ݒ�
	mScale = scale;	//�g�k�̐ݒ�
	CTransform::Update();	//�s��̍X�V
	//�D��x��1�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this); //�폜����
	CTaskManager::Get()->Add(this); //�ǉ�����
	//�ڕW�n�_�̐ݒ�
	//mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
	mPoint = CPlayer::spThis->mPosition;
}

//�X�V����
void CEnemy2::Update() {

	//HP��0�ȉ��̎��@���j
	if (mHp <= 0)
	{
		mHp--;
		//15�t���[�����ɃG�t�F�N�g
		if (mHp % 15 == 0)
		{
			//�G�t�F�N�g����
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		//���~������
		mPosition.mY -= 0.03f;
		CTransform::Update();	//�s��X�V
		return;	//�Ăь��֖߂�
	}


	//�������iX���j�̃x�N�g�������߂�
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//������iY���j�̃x�N�g�������߂�
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//�O�����iZ���j�̃x�N�g�������߂�
	CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

	//�ڕW�n�_�܂ł̃x�N�g�������߂�
	CVector vp = mPoint - mPosition;
	float dx = vp.Dot(vx);	//���x�N�g���Ƃ̓��ς����߂�
	float dy = vp.Dot(vy);	//��x�N�g���Ƃ̓��ς����߂�
	float dz = vp.Dot(vz);
	float margin = 1.0f;
	//���E�����։�]
	if (dx > margin)
	{
		mRotation.mY += 1.0f;
	}
	else if (dx < -margin)
	{
		mRotation.mY -= 1.0f;
	}
	//�㉺�����։�]
	if (dy > margin)
	{
		mRotation.mX -= 1.0f;
	}
	else if (dy < -margin)
	{
		mRotation.mX += 1.0f;
	}
	if (mAttackFlg == false){
		if (dz>0.0f&&-3.0f < dx&&dx < 3.0f){
			mPosition = mPosition + vp.Normalize() * VELOCITY;
			mAcquisitionFlg = true;
		}
		else{
			mAcquisitionFlg = false;
		}
	}

	//�ړ�����
	//mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;

	CTransform::Update();	//�s��X�V

	//���悻3�b���ɖڕW�n�_���X�V
	int r = rand() % 30;	//rand()�͐����̗�����Ԃ�
							//% 180 ��180�Ŋ������]������߂�
	if (r == 0)
	{
		if (mpPlayer)
		{
			mPoint = mpPlayer->mPosition;
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}

	mpPlayer = 0;
}
//�Փˏ���
//Collision(�R���C�_1, �R���C�_2)
void CEnemy2::Collision(CCollider *m, CCollider *o) {
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//�������T�[�`�p�̎�
	if (m->mTag == CCollider::ESEARCH)
	{
		//���肪���R���C�_�̎�
		if (o->mType == CCollider::ESPHERE)
		{
			//���肪�v���C���[�̎�
			if (o->mpParent->mTag == EPLAYER)
			{
				//�Փ˂��Ă��鎞
				if (CCollider::Collision(m, o))
				{
					//�v���C���[�̃|�C���^�ݒ�
					mpPlayer = o->mpParent;
				}
			}
		}
		return;
	}
	//�G�̍U�������Ƀv���C���[�����邩�ǂ����̔���
	if (m->mTag == CCollider::EENEMY_ATTACK_RANGE){
		if (o->mType == CCollider::ESPHERE){
			if (o->mpParent->mTag == EPLAYER){
				if (CCollider::Collision(m, o)){
					mAttackFlg = true;
				}
				else{
					mAttackFlg = false;
				}
			}
		}
		return;
	}

	switch (o->mType)
	{
	case CCollider::ESPHERE:
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (o->mTag == CCollider::EPLAYER_ATTACK){
			if (CPlayer::spThis->mAttackCount > 0){
				if (CCollider::Collision(m, o)) {
					//�G�t�F�N�g����
					new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
					mHp--;	//�q�b�g�|�C���g�̌��Z
				}
			}
		}
		break;
	case CCollider::ETRIANGLE:	//�O�p�R���C�_�̎�
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{	//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
			mPosition = mPosition + adjust;
			//���j�Œn�ʂɏՓ˂���Ɩ���
			if (mHp <= 0)
			{
				mEnabled = false;
			}
		}
		break;
	}

	return;

	switch (m->mTag)
	{
	case CCollider::ESEARCH:
		if (o->mType == CCollider::ESPHERE)
		{
			if (o->mpParent->mTag == EPLAYER)
			{
				//�R���C�_��m��y���Փ˂��Ă��邩����
				if (CCollider::Collision(m, o))
				{
					//�v���C���[�̃|�C���^��ݒ�
					mpPlayer = o->mpParent;
				}
			}
		}
		break;
	default:	//�{�̂̎�
		switch (o->mType)
		{
		case CCollider::ESPHERE:
			if (o->mTag == CCollider::EBODY)
			{
				//�R���C�_��m��y���Փ˂��Ă��邩����
				if (CCollider::Collision(m, o)) {
					//�G�t�F�N�g����
					new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
				}
			}
			break;
		case CCollider::ETRIANGLE:	//�O�p�R���C�_�̎�
			CVector adjust; //�����l
			//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{	//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
				mPosition = mPosition + adjust;
			}
			break;
		}
	}

	return;


	if (o->mTag == CCollider::ESEARCH)
		return;
	//����̃R���C�_�^�C�v�̔���
	switch (o->mType)
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o)) {
			switch (m->mTag)
			{
			case CCollider::ESEARCH:
				if (o->mpParent->mTag == EPLAYER)
				{
					//�v���C���[�̃|�C���^��ݒ�
					mpPlayer = o->mpParent;
				}
				break;
			default:
				//�G�t�F�N�g����
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�
		if (m->mTag == CCollider::EBODY)
		{
			CVector adjust; //�����l
			//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{	//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
				mPosition = mPosition + adjust;
			}
		}
		break;
	}
}

void CEnemy2::TaskCollision()
{
	mCollider.ChangePriority();
	mColSearch.ChangePriority();
	mColAttackRange.ChangePriority();
	mColAttack.ChangePriority();
//	mCollider3.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColAttackRange, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColAttack, COLLISIONRANGE);
//	CCollisionManager::Get()->Collision(&mCollider3, COLLISIONRANGE);
}
