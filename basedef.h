//
// Created by hoenking on 11/27/19.
//

#ifndef C__LEARN_BASEDEF_H
#define C__LEARN_BASEDEF_H

//添加类成员变量并提供存取函数
#define ADD_CLASS_FIELD(type, name, getter, setter) \
private:\
    type m_##name;\
public:\
    type& getter(){return m_##name;}\
    type const & getter() const {return m_##name;}\
    void setter(type name){m_##name=name;}


#endif //C__LEARN_BASEDEF_H
