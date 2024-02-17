#ifndef SKETCHFACTORY_H
#define SKETCHFACTORY_H

typedef void (*fp_void_no_parameters)();

class SketchFactory {
public:
    SketchFactory();
    bool loadSketch(const char* compiledSketchSOfilename);
    void setup() const;
    void loop() const;
    ~SketchFactory();

private:
    void * handler;
    fp_void_no_parameters fp_setup, fp_loop;
    static void Reset_dlerror();
    static void Check_dlerror();
};

#endif //SKETCHFACTORY_H
