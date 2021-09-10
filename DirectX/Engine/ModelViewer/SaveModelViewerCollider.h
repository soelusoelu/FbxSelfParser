#pragma once

class GameObject;

class SaveModelViewerCollider {
private:
    SaveModelViewerCollider() = delete;
    ~SaveModelViewerCollider() = delete;

public:
    static void save(const GameObject& target);

private:
    SaveModelViewerCollider(const SaveModelViewerCollider&) = delete;
    SaveModelViewerCollider& operator=(const SaveModelViewerCollider&) = delete;
};
