#ifndef S21_CONTROLLER
#define S21_CONTROLLER

#include <string>
#include <vector>

#include "../commons/s21_objects.hh"
#include "../model/s21_Facade.hh"
#include "../model/s21_WireframeModel.hh"

namespace s21 {

class Controller {
 public:
  Controller() = default;

  void LoadModel(const std::string& filename) {
    _model_facade.loadModel(filename);
  }
  void TranslateModel(double dx, double dy, double dz) {
    _model_facade.applyTranslation(dx, dy, dz);
  }
  void RotateModel(double angle_x, double angle_y, double angle_z) {
    _model_facade.applyRotation(angle_x, angle_y, angle_z);
  }
  void ScaleModel(double scale) { _model_facade.applyScaling(scale); }
  const Mesh& GetData() const { return _model_facade.getMesh(); }

  ModelFacade& GetModelFacade() { return _model_facade; }

 private:
  ModelFacade _model_facade;
};

}  // namespace s21

#endif
