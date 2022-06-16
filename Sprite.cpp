#include "Sprite.h"

//void Sprite::Update(const Math::Vector3& pos = { 0.0f,0.0f,0.0f }, const Math::Vector3& rot = { 0.0f,0.0f,0.0f }, const Math::Vector3& scale = { 1.0f,1.0f,1.0f }) {
//
//	Math::Matrix4 matTrans, matRot, matScale;
//
//	// 平行移動行列
//	matTrans.MakeTranslation(pos);
//	// 回転行列
//	matRot.MakeRotation(rot);
//	// スケール行列
//	matScale.MakeScaling(scale);
//	// ワールド行列
//	constMapTransform->matWorld = matScale * matRot * matTrans;
//}