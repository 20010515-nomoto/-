#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy :public CXCharacter{
public:
	CXEnemy();
	void Init(CModelX *model);
	void Update();
	CCollider mEnemyColSphereBody;	//��
	CCollider mEnemyColSphereHead;	//��
	CCollider mEnemyColSphereSword0;	//��
	CCollider mEnemyColSphereSword1;	//��
	CCollider mEnemyColSphereSword2;	//��

	CCollider mColSearch;	//�T�[�`�p�R���C�_
	CCollider mColAttack_Range;	//�U���͈�

	//�Փˏ���
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();

	bool mAcquisitionFlg;	//�ߑ��t���O:�G���v���C���[��ߑ����Ă��邩�ǂ��� true:���Ă���
	bool mAttackFlg;	//�U���t���O:true �U������

	int mHp;	//�q�b�g�|�C���g
	CCharacter *mpPlayer;	//�v���C���[�̃|�C���^
	CVector mPoint;	//�ڕW�n�_

	static CXEnemy *spThis;
};

#endif