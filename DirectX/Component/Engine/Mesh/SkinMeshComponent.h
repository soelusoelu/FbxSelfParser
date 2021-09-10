#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IAnimation.h"
#include "../../../Mesh/Motion.h"
#include <functional>
#include <string>
#include <vector>

class MeshShader;

class SkinMeshComponent
    : public Component
{
public:
    SkinMeshComponent();
    ~SkinMeshComponent();
    virtual void update() override;

    //モーション名を設定する
    void setMotionName(const std::string& name, unsigned motionNo);
    //モーション番号からモーションを変更する
    void changeMotion(unsigned motionNo);
    //モーション名からモーションを変更する
    void changeMotion(const std::string& motionName);
    //姿勢をTポーズにする
    void tPose();
    //モーションを更新するか設定する
    void setMotionUpdateFlag(bool value);
    //モーションを更新するか取得する
    bool getMotionUpdateFlag() const;
    //モーションのループ状態を指定する
    void setLoop(bool value);
    //モーションのループ状態を取得する
    bool getLoop() const;
    //モーション数を取得する
    int getMotionCount() const;
    //モーション番号からモーションを取得する
    const Motion& getMotion(unsigned motionNo) const;
    //モーション名からモーションを取得する
    const Motion& getMotion(const std::string& motionName) const;
    //現在のモーションを取得する
    const Motion& getCurrentMotion() const;
    //現在のモーション番号を取得する
    int getCurrentMotionNumber() const;
    //現在のモーションのフレーム数を取得する
    int getCurrentMotionFrame() const;
    //現在のボーン姿勢行列を取得する
    const std::vector<Matrix4>& getBoneCurrentFrameMatrix() const;
    //各種インターフェースを設定する
    void setValue(const std::shared_ptr<MeshShader>& meshShader, IAnimation* anim);
    //changeMotion後に呼ばれるコールバック
    void callbackChangeMotion(const std::function<void()>& callback);
    //このフレームでのボーン姿勢計算後に呼ばれるコールバック
    void callbackComputeCurrentBones(const std::function<void()>& callback);

private:
    SkinMeshComponent(const SkinMeshComponent&) = delete;
    SkinMeshComponent& operator=(const SkinMeshComponent&) = delete;

    void calcNextPose();
    void calcCurrentFrame();

public:
    static constexpr int T_POSE_NO = -1;

private:
    //アニメーションインターフェース
    IAnimation* mAnimation;
    //メッシュ用シェーダーコンポーネント
    std::shared_ptr<MeshShader> mMeshShader;
    //現在のボーン姿勢
    std::vector<Matrix4> mCurrentBones;
    //changeMotion後に呼ばれるコールバック
    Function<void()> mCallbackChangeMotion;
    //ボーン姿勢計算後に呼ばれるコールバック
    Function<void()> mCallbackComputeCurrentBones;
    //現在のモーション番号
    int mCurrentMotionNo;
    //現在のモーションフレーム
    int mCurrentFrame;
    //モーションを更新するか
    bool mIsMotionUpdate;
    //モーションを繰り返すか
    bool mIsLoop;
};
