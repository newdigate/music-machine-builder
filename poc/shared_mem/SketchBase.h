#ifndef SKETCHBASE_H
#define SKETCHBASE_H

class SketchBase {
public:
    virtual ~SketchBase() = default;
    virtual void foo() const {
    };
private:
};

using SketchBase_creator_t = SketchBase *(*)();
#endif //SKETCHBASE_H
