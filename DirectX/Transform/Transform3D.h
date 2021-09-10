#pragma once

#include "../Device/Function.h"
#include "../Math/Math.h"
#include "../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <functional>
#include <memory>

class GameObject;
class ParentChildRelationship;

class Transform3D {
public:
    Transform3D(GameObject* gameObject = nullptr);
    ~Transform3D();

    //行列を計算する
    void computeMatrix();
    //ローカル行列の取得
    const Matrix4& getLocalTransform() const;
    //ワールド行列の取得
    const Matrix4& getWorldTransform() const;

    //位置の設定
    void setPosition(const Vector3& pos);
    //親子関係を考慮した位置の取得
    Vector3 getPosition() const;
    //ローカル位置の取得
    const Vector3& getLocalPosition() const;
    //translationの方向と距離に移動する
    void translate(const Vector3& translation);
    //x, y, z分移動する
    void translate(float x, float y, float z);

    //回転量の設定
    void setRotation(const Quaternion& rot);
    //軸を中心に回転量を設定
    void setRotation(const Vector3& axis, float angle);
    //オイラー角で回転量を設定
    void setRotation(const Vector3& eulers);
    //親子関係を考慮した回転量の取得
    Quaternion getRotation() const;
    //ローカル回転量の取得
    const Quaternion& getLocalRotation() const;
    //軸を中心に回転させる
    void rotate(const Vector3& axis, float angle);
    //eulers量回転させる
    void rotate(const Vector3& eulers);
    //targetを向く
    void lookAt(const Transform3D& target, const Vector3& upwards = Vector3::up);

    //ピボット位置の設定
    void setPivot(const Vector3& pivot);
    //ピボット位置の取得
    const Vector3& getPivot() const;

    //スケール値の設定
    void setScale(const Vector3& scale);
    //均等に拡縮する
    void setScale(float scale);
    //親子関係を考慮したスケール値の取得
    Vector3 getScale() const;
    //ローカルスケール値の取得
    const Vector3& getLocalScale() const;

    //回転量に応じた前方ベクトルの取得
    Vector3 forward() const;
    //回転量に応じた上ベクトルの取得
    Vector3 up() const;
    //回転量に応じた右ベクトルの取得
    Vector3 right() const;

    //ゲームオブジェクトを取得する
    GameObject& gameObject() const;
    //親子関係統括クラスを取得する
    ParentChildRelationship& getParentChildRelation() const;

    //ワールド行列計算前に呼ばれる
    void callbackBeforeComputeWorldMatrix(const std::function<void()>& f);

    //ロード/セーブ
    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);

    //インスペクター
    void drawInspector();

private:
    Transform3D(const Transform3D&) = delete;
    Transform3D& operator=(const Transform3D&) = delete;

    //ローカル行列を計算する
    void computeLocalMatrix();
    //ワールド行列を計算する
    void computeWorldMatrix();
    //子のワールド行列を計算する
    void computeChildrenTransform();

private:
    GameObject* mGameObject;
    std::unique_ptr<ParentChildRelationship> mParentChildRelation;
    Matrix4 mLocalTransform;
    Matrix4 mWorldTransform;
    Vector3 mPosition;
    Quaternion mRotation;
    Vector3 mPivot;
    Vector3 mScale;
    Function<void()> mCallbackBeforeComputeWorldMatrix;
};
