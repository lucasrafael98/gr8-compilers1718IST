#ifndef __GR8_SEMANTICS_SYMBOL_H__
#define __GR8_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace gr8 {

  class symbol {
    basic_type *_type;
    std::string _name;
    std::vector<unsigned long int> *_funcArgs; //for checking the args on func calls
    double _value; // hack!(?)
    int _offset; // for ENTER x and LOCAL <offset> ops.
    int _accessMode; // for determining whether it's local/global/extern (0/1/2).
    bool _isFunction;

  public:
    symbol(basic_type *type, const std::string &name, double value, int accessMode, bool isFunction) :
        _type(type), _name(name), _value(value), _accessMode(accessMode), _isFunction(isFunction) {
          _funcArgs = new std::vector<unsigned long int>();
    }

    virtual ~symbol() {
      delete _type;
      delete _funcArgs;
    }

    basic_type *type() const {
      return _type;
    }
    const std::string &name() const {
      return _name;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    int offset() const{
      return _offset;
    }
    int offset(int offset){
      return _offset = offset;
    }
    int accessMode() const{
      return _accessMode;
    }
    bool isFunction() const{
      return _isFunction;
    }
    void addArg(unsigned long int type){
      _funcArgs->push_back(type);
    }
    unsigned long int arg(int i){
      return _funcArgs->at(i);
    }
    std::vector<unsigned long int>* args(){
      return _funcArgs;
    }
  };

} // gr8

#endif
