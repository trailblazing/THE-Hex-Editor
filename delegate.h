#ifndef DELEGATE_H_INCLUDED
#define DELEGATE_H_INCLUDED

#include <iostream>
#include <exception>
namespace sd {

template<typename bailee, typename bailee_return, typename bailee_require>
struct _invoke_functor {
    //bailee *const obj;
    //typedef bailee_return (bailee::* function_pointer)(bailee *const, bailee_require);

    bailee_return(bailee::*inv)(bailee_require);

    _invoke_functor( bailee_return(bailee::*_inv)(bailee_require) ) {
        inv = _inv;
    }

    void (*void_pr)(void * const _obj, void const * _bailee_requiree);
    //void (*)(void * const, void *const) _transmit(){void_pr=(void (*)(void * const, void *const))&excute;return void_pr;}
    bailee_return excute(void * const _obj, void const * _bailee_requiree) {
        // obj = _obj;
        return ((static_cast<bailee * const>(_obj))->*inv)(static_cast<bailee_require * const>(_bailee_requiree));
    }

};

template<typename caller_require, typename bailee, typename bailee_return, typename bailee_require>
struct _invoke_functor_return {

    caller_require(bailee::*inv)(bailee_require);

    _invoke_functor_return( caller_require(bailee::*_inv)(bailee_require) ) {
        inv = _inv;
    }

};

template<typename FT>
struct _invoke_functor_return_easy {

    FT * inv;//caller_require*(bailee::*inv)(bailee_require);

    _invoke_functor_return_easy(FT *_inv) {
        inv = _inv;
    }

};

template<typename caller_require, typename bailee, typename bailee_return, typename bailee_require_0, typename bailee_require_1>
struct _invoke_functor_return_double_in {

    caller_require(bailee::*inv)(bailee_require_0, bailee_require_1);

    _invoke_functor_return_double_in( caller_require(bailee::*_inv)(bailee_require_0, bailee_require_1) ) {
        inv = _inv;
    }

};


template<typename caller_require, typename bailee, typename bailee_return>
struct _invoke_functor_return_read {

    caller_require(bailee::*inv)();

    _invoke_functor_return_read( caller_require(bailee::*_inv)() ) {
        inv = _inv;
    }

};

template<typename bailee, typename bailee_return, typename bailee_require>
bailee_return _transmit(void * _caller_require, void * const _obj, void * const _invoke_obj, void const * _bailee_requiree)
{
    //bailee_return (bailee::*inv) (bailee_require) = (static_cast<_invoke_functor<bailee, bailee_return, bailee_require> * const>(_invoke_obj))->inv;
    //((static_cast<bailee * const>(_obj)) ->* inv) (*(static_cast<bailee_require * const>(_p)));
    //bailee_return * result = static_cast<bailee_return *>(_caller_require);
    return *static_cast<bailee_return *>(_caller_require) = ((static_cast<bailee * const>(_obj)) ->* ((static_cast<_invoke_functor<bailee, bailee_return, bailee_require> * const>(_invoke_obj))->inv)) (*(static_cast<bailee_require const *>(_bailee_requiree)));

}

template<typename caller_require, typename bailee, typename bailee_return, typename bailee_require>
caller_require _transmit_return(void * const _obj, void * const _invoke_obj, void * _bailee_requiree)
{
    return static_cast<caller_require>(((static_cast<bailee * const>(_obj)) ->* ((static_cast<_invoke_functor_return<caller_require, bailee, bailee_return, bailee_require> * const>(_invoke_obj))->inv)) (*(static_cast<bailee_require *>(_bailee_requiree))));

}

template<typename caller_require, typename bailee, typename FT, typename bailee_require>
caller_require _transmit_return_easy(void * const _obj, void * const _invoke_obj, void const * _bailee_requiree)
{
    return static_cast<caller_require>(((static_cast<bailee * const>(_obj)) ->* ((static_cast<_invoke_functor_return_easy<FT> * const>(_invoke_obj))->inv)) (*(static_cast<bailee_require const *>(_bailee_requiree))));

}

template<typename caller_require, typename bailee, typename bailee_return, typename bailee_require_0, typename bailee_require_1>
caller_require _transmit_return_double_in(void * const _obj, void * const _invoke_obj, void  * const _bailee_requiree_0, void  * const _bailee_requiree_1)
{
    return static_cast<caller_require>(((static_cast<bailee * const>(_obj)) ->* ((static_cast<_invoke_functor_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1> * const>(_invoke_obj))->inv)) (*(static_cast<bailee_require_0 *const>(_bailee_requiree_0)), *(static_cast<bailee_require_1 *const>(_bailee_requiree_1))));

}

template<typename caller_require, typename bailee, typename bailee_return>
caller_require _transmit_return_read(void * const _obj, void * _invoke_obj)
{
    return static_cast<caller_require>(
               (
                   (static_cast<bailee * const>(_obj)) ->* (
                       (
                           static_cast<_invoke_functor_return_read<caller_require, bailee, bailee_return> *>(_invoke_obj)
                       )->inv
                   )
               )
               ()
           );

}

template<typename bailee, typename bailee_return, typename bailee_require>
void _transmit(void * _caller_require, void * const _obj, void * const _invoke_obj, void const * _bailee_requiree)
{
    /*
    try{
        ((static_cast<bailee * const>(_obj)) ->* ((static_cast<_invoke_functor<bailee, void, bailee_require> * const>(_invoke_obj))->inv)) (*(static_cast<bailee_require * const>(_bailee_requiree)));
    }
    catch(exception & e)
    {
        cout<<e.what()<<endl;
        return 1;
    }

    return 0;
    */

    static_cast<bailee_return *>(_caller_require) = ((static_cast<bailee * const>(_obj)) ->* ((static_cast<_invoke_functor<bailee, void, bailee_require> * const>(_invoke_obj))->inv)) (*(static_cast<bailee_require const *>(_bailee_requiree)));

}

template<typename bailee, typename bailee_return, typename bailee_require>
void _transmit_input(void * const _obj, void * const _invoke_obj, void const * _bailee_requiree)
{
    ((static_cast<bailee * const>(_obj)) ->* ((static_cast<_invoke_functor<bailee, void, bailee_require> * const>(_invoke_obj))->inv)) (*(static_cast<bailee_require const *>(_bailee_requiree)));

}

template<typename bailee, typename bailee_return, typename bailee_require>
void _release(void * _invoke_functee)
{
    //if(NULL != _invoke_functee )
    delete static_cast<_invoke_functor<bailee, bailee_return, bailee_require> *>(_invoke_functee);

}
template<typename caller_require, typename bailee, typename bailee_return, typename bailee_require>
void _release_return(void * _invoke_functee)
{
    //if(NULL != _invoke_functee )
    delete static_cast<_invoke_functor_return<caller_require, bailee, bailee_return, bailee_require> *>(_invoke_functee);

}

template<typename FT>
void _release_return_easy(void * _invoke_functee)
{
    //if(NULL != _invoke_functee )
    delete static_cast<_invoke_functor_return_easy<FT> *>(_invoke_functee);

}


template<typename caller_require, typename bailee, typename bailee_return, typename bailee_require_0, typename bailee_require_1>
void _release_return_double_in(void * _invoke_functee)
{
    //if(NULL != _invoke_functee )
    delete static_cast<_invoke_functor_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1> *>(_invoke_functee);

}


template<typename caller_require, typename bailee, typename bailee_return>
void _release_return_read(void * _invoke_functee)
{
    //if(NULL != _invoke_functee )
    delete static_cast<_invoke_functor_return_read<caller_require, bailee, bailee_return> *>(_invoke_functee);

}

template<typename bailee, typename bailee_require>
void _release(void * _invoke_functee)
{
    //if(NULL != _invoke_functee )
    delete static_cast<_invoke_functor<bailee, void, bailee_require> *>(_invoke_functee);

}


class Agency {

protected:

    //void(*inv)(void *const, void(*)(void *const), void *const);
    void(*inv)(void * _caller_require, void * const _obj, void * const _invoke_obj, void const * _bailee_requiree);
    void(*del)(void *);

    //void (*inv_function)(void *const);
    void * const obj;
    //void(* invoke_functee)(void *const);
    void * invoke_functee;
    void * bailee_requiree;
    void * caller_requiree;
    //void * return_bailee_requiree;
public:


    template<typename bailee, typename bailee_return, typename bailee_require>	// _Convention see msdn: Argument Passing and Naming Conventions
    Agency(bailee_return &caller_require, bailee & _obj, bailee_return(bailee::*_inv)(bailee_require) , bailee_require & input_pamater)
        : obj(& _obj) {

        //_____________________________________________________________________________________________
        try {
            if(!obj) {
                cerr<<endl<<"empty object!"<<endl;
                cin.get();
                //exit(1);
            }
        } catch(exception e) {
            throw e;
        }

        invoke_functee = (void *)0;
        invoke_functee = (void *) new _invoke_functor<bailee, bailee_return, bailee_require>(_inv);
        del = _release<bailee, bailee_return, bailee_require>;
        bailee_requiree = (void *) & input_pamater;
        caller_requiree = (void *) & caller_require;
        inv = _transmit<bailee, bailee_return, bailee_require>;

        //_____________________________________________________________________________________________

    }

    // not safety, [&&&^%^&##$%^%^^&%^$$#$#&^&**]
    template<typename bailee, typename bailee_return, typename bailee_require>
    void strategy(bailee_return caller_require, bailee_return (bailee::*_inv)(bailee_require), bailee_require & input_pamater) {

        if(NULL != invoke_functee ) delete static_cast<_invoke_functor<bailee, bailee_return, bailee_require> *>(invoke_functee);
        _invoke_functor<bailee, bailee_return, bailee_require> *invoke = new _invoke_functor<bailee, bailee_return, bailee_require>(_inv);
        invoke_functee = (void *)invoke;
        del = _release<bailee, bailee_return, bailee_require>;
        bailee_requiree = (void *) & input_pamater;
        caller_requiree = (void *) & caller_require;
        inv = _transmit<bailee, bailee_return, bailee_require>;

    }

    void operator()() {	// he can return bailee_require !!!, if bailee_require is as same as object type? what happen?
        inv(caller_requiree, obj, invoke_functee, bailee_requiree);

        //return obj->inv(p);
        // if you cast bailee_require as other type not as same as in constructor give InvokeByPointer
        // , no problem, it can not pass compile->right idea
        // , only unify with static Wedge member in constructor->wrong idea
    }


    ~Agency() {
        if(NULL != invoke_functee )
            del(invoke_functee);
    }

};

template<typename caller_require>
class AgencyReturnDoubleIn {

protected:

    //void(*inv)(void *const, void(*)(void *const), void *const);
    caller_require (*inv)(void * const _obj, void * const _invoke_obj, void *const _bailee_requiree_0, void *const _bailee_requiree_1);
    void(*del)(void *);

    //void (*inv_function)(void *const);
    void * const obj;
    //void(* invoke_functee)(void *const);
    void * invoke_functee;
    //void * bailee_requiree_0;
    //void * bailee_requiree_1;
    //void * caller_requiree;
    //void * return_bailee_requiree;
public:


    template<typename bailee, typename bailee_return, typename bailee_require_0, typename bailee_require_1>	// _Convention see msdn: Argument Passing and Naming Conventions
    AgencyReturnDoubleIn(bailee & _obj
                         , bailee_return(bailee::*_inv)(bailee_require_0, bailee_require_1)
//                         , bailee_require_0 & input_pamater_0
//                         , bailee_require_1 & input_pamater_1
                        )
        : obj(& _obj) {

        //_____________________________________________________________________________________________
        try {
            if(!obj) {
                cerr<<endl<<"empty object!"<<endl;
                cin.get();
                //exit(1);
            }
        } catch(exception e) {
            throw e;
        }

        invoke_functee = (void *)0;
        invoke_functee = (void *) new _invoke_functor_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1>(_inv);
        del = _release_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1>;
        //bailee_requiree_0 = (void *) & input_pamater_0;
        //bailee_requiree_1 = (void *) & input_pamater_1;
        //caller_requiree = (void *) & caller_require;
        inv = _transmit_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1>;

        //_____________________________________________________________________________________________

    }


    // not safety, [&&&^%^&##$%^%^^&%^$$#$#&^&**]
    template<typename bailee, typename bailee_return, typename bailee_require_0, typename bailee_require_1>
    void strategy(bailee_return (bailee::*_inv)(bailee_require_0, bailee_require_1)
//                  , bailee_require_0 & input_pamater_0
//                  , bailee_require_1 & input_pamater_1
                 ) {

        if(NULL != invoke_functee ) delete static_cast<_invoke_functor_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1> *>(invoke_functee);
        _invoke_functor_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1> *invoke = new _invoke_functor_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1>(_inv);
        invoke_functee = (void *)invoke;
        del = _release_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1>;
        //bailee_requiree_0 = (void *) & input_pamater_0;
        //bailee_requiree_1 = (void *) & input_pamater_1;
        //caller_requiree = (void *) & caller_require;
        inv = _transmit_return_double_in<caller_require, bailee, bailee_return, bailee_require_0, bailee_require_1>;

    }

//    template<typename bailee_require_0, typename bailee_require_1>
//    caller_require operator()(bailee_require_0 &bailee_requiree_0, bailee_require_1 &bailee_requiree_1) {	// he can return bailee_require !!!, if bailee_require is as same as object type? what happen?
//        return inv(obj, invoke_functee, bailee_requiree_0, bailee_requiree_1);
//
//        //return obj->inv(p);
//        // if you cast bailee_require as other type not as same as in constructor give InvokeByPointer
//        // , no problem, it can not pass compile->right idea
//        // , only unify with static Wedge member in constructor->wrong idea
//    }

    template<typename bailee_require_0, typename bailee_require_1>
    caller_require operator()(bailee_require_0 &bailee_requiree_0, bailee_require_1 &bailee_requiree_1) {	// he can return bailee_require !!!, if bailee_require is as same as object type? what happen?
        return inv(obj, invoke_functee, &bailee_requiree_0, &bailee_requiree_1);
    }


    ~AgencyReturnDoubleIn() {
        if(NULL != invoke_functee )
            del(invoke_functee);
    }

};

template<typename caller_require>
class AgencyReturn {

protected:

    //void(*inv)(void *const, void(*)(void *const), void *const);
    caller_require (*inv)(void * const _obj, void * const _invoke_obj, void * _bailee_requiree);
    void(*del)(void *);

    //void (*inv_function)(void *const);
    void * const obj;
    //void(* invoke_functee)(void *const);
    void * invoke_functee;
    //void * bailee_requiree;
    //void * caller_requiree;
    //void * return_bailee_requiree;
public:


    template<typename bailee, typename bailee_return, typename bailee_require>	// _Convention see msdn: Argument Passing and Naming Conventions
    AgencyReturn(bailee & _obj, bailee_return(bailee::*_inv)(bailee_require))//AgencyReturn(bailee & _obj, bailee_return(bailee::*_inv)(bailee_require) , bailee_require & input_pamater)
        : obj(& _obj) {

        //_____________________________________________________________________________________________
        try {
            if(!obj) {
                cerr<<endl<<"empty object!"<<endl;
                cin.get();
                //exit(1);
            }
        } catch(exception e) {
            throw e;
        }

        invoke_functee = (void *)0;
        invoke_functee = (void *) new _invoke_functor_return<caller_require, bailee, bailee_return, bailee_require>(_inv);
        del = _release_return<caller_require, bailee, bailee_return, bailee_require>;
        //bailee_requiree = (void *) & input_pamater;
        //caller_requiree = (void *) & caller_require;
        inv = _transmit_return<caller_require, bailee, bailee_return, bailee_require>;

        //_____________________________________________________________________________________________

    }


    // not safety, [&&&^%^&##$%^%^^&%^$$#$#&^&**]
    template<typename bailee, typename bailee_return, typename bailee_require>
    void strategy(bailee_return (bailee::*_inv)(bailee_require)) { //void strategy(bailee_return (bailee::*_inv)(bailee_require), bailee_require & input_pamater)

        if(NULL != invoke_functee ) delete static_cast<_invoke_functor_return<caller_require, bailee, bailee_return, bailee_require> *>(invoke_functee);
        _invoke_functor_return<caller_require, bailee, bailee_return, bailee_require> *invoke = new _invoke_functor_return<caller_require, bailee, bailee_return, bailee_require>(_inv);
        invoke_functee = (void *)invoke;
        del = _release_return<caller_require, bailee, bailee_return, bailee_require>;
        //bailee_requiree = (void *) & input_pamater;
        //caller_requiree = (void *) & caller_require;
        inv = _transmit_return<caller_require, bailee, bailee_return, bailee_require>;

    }
    template<typename bailee_require>
    caller_require operator()(bailee_require & input_pamater) {	// he can return bailee_require !!!, if bailee_require is as same as object type? what happen?
        return inv(obj, invoke_functee, &input_pamater);

        //return obj->inv(p);
        // if you cast bailee_require as other type not as same as in constructor give InvokeByPointer
        // , no problem, it can not pass compile->right idea
        // , only unify with static Wedge member in constructor->wrong idea
    }


    ~AgencyReturn() {
        if(NULL != invoke_functee )
            del(invoke_functee);
    }

};

template<typename caller_require, typename bailee_require>
class AgencyReturnEasy {
protected:

    caller_require (*inv)(void * const _obj, void * const _invoke_obj, bailee_require const * _bailee_requiree);
    void(*del)(void *);
    void * const obj;
    void * invoke_functee;

public:


    template<typename bailee, typename FT>	// _Convention see msdn: Argument Passing and Naming Conventions
    AgencyReturnEasy(bailee & _obj, FT*_inv)//AgencyReturn(bailee & _obj, bailee_return(bailee::*_inv)(bailee_require) , bailee_require & input_pamater)
        : obj(& _obj) {

        //_____________________________________________________________________________________________
        try {
            if(!obj) {
                cerr<<endl<<"empty object!"<<endl;
                cin.get();
                //exit(1);
            }
        } catch(exception e) {
            throw e;
        }

        invoke_functee = (void *)0;
        invoke_functee = (void *) new _invoke_functor_return_easy<FT>(_inv);
        del = _release_return_easy<FT>;
        inv = _transmit_return_easy<caller_require, bailee, FT, bailee_require>;

        //_____________________________________________________________________________________________

    }


    // not safety, [&&&^%^&##$%^%^^&%^$$#$#&^&**]
    template<typename bailee, typename FT>
    void strategy(FT *_inv) { //void strategy(bailee_return (bailee::*_inv)(bailee_require), bailee_require & input_pamater)

        if(NULL != invoke_functee ) delete static_cast<_invoke_functor_return_easy<FT> *>(invoke_functee);
        _invoke_functor_return_easy<FT> *invoke = new _invoke_functor_return_easy<FT>(_inv);
        invoke_functee = (void *)invoke;
        del = _release_return_easy<FT>;
        inv = _transmit_return_easy<caller_require, bailee, FT, bailee_require>;

    }

    caller_require operator()(bailee_require &input_pamater) {	// he can return bailee_require !!!, if bailee_require is as same as object type? what happen?
        return inv(obj, invoke_functee, &input_pamater);
    }


    ~AgencyReturnEasy() {
        if(NULL != invoke_functee )
            del(invoke_functee);
    }

};

template<typename caller_require>
struct AgencyRead {
protected:

    //void(*inv)(void *const, void(*)(void *const), void *const);
    caller_require (*inv)(void * const _obj, void * _invoke_obj);
    void(*del)(void *);

    //void (*inv_function)(void *const);
    void * const obj;
    //void(* invoke_functee)(void *const);
    void * invoke_functee;
    //void * bailee_requiree;
    //void * caller_requiree;
    //void * return_bailee_requiree;
public:

    template<typename bailee, typename bailee_return>	// _Convention see msdn: Argument Passing and Naming Conventions
    AgencyRead(bailee & _obj, bailee_return(bailee::*_inv)())
        : obj(& _obj) {

        //_____________________________________________________________________________________________
        try {
            if(!obj) {
                cerr<<endl<<"empty object!"<<endl;
                cin.get();
                //exit(1);
            }
        } catch(exception e) {
            throw e;
        }

        invoke_functee = (void *)0;
        invoke_functee = (void *) new _invoke_functor_return_read<caller_require, bailee, bailee_return>(_inv);
        del = _release_return_read<caller_require, bailee, bailee_return>;
        //bailee_requiree = (void *) & input_pamater;
        //caller_requiree = (void *) & caller_require;
        inv = _transmit_return_read<caller_require, bailee, bailee_return>;

        //_____________________________________________________________________________________________

    }


    // not safety, [&&&^%^&##$%^%^^&%^$$#$#&^&**]
    template<typename bailee, typename bailee_return>
    void strategy(bailee_return (bailee::*_inv)()) {

        if(NULL != invoke_functee ) delete static_cast<_invoke_functor_return_read<caller_require, bailee, bailee_return> *>(invoke_functee);
        _invoke_functor_return_read<caller_require, bailee, bailee_return> *invoke = new _invoke_functor_return_read<caller_require, bailee, bailee_return>(_inv);
        invoke_functee = (void *)invoke;
        del = _release_return_read<caller_require, bailee, bailee_return>;
        //bailee_requiree = (void *) & input_pamater;
        //caller_requiree = (void *) & caller_require;
        inv = _transmit_return_read<caller_require, bailee, bailee_return>;

    }

    caller_require operator()() {	// he can return bailee_require !!!, if bailee_require is as same as object type? what happen?
        return inv(obj, invoke_functee);

        //return obj->inv(p);
        // if you cast bailee_require as other type not as same as in constructor give InvokeByPointer
        // , no problem, it can not pass compile->right idea
        // , only unify with static Wedge member in constructor->wrong idea
    }


    ~AgencyRead() {
        if(NULL != invoke_functee )
            del(invoke_functee);
    }
};


class AgencyInput {

protected:

    //void(*inv)(void *const, void(*)(void *const), void *const);
    void(*inv)(void * const _obj, void * const _invoke_obj, void const * _bailee_requiree);
    void(*del)(void *);

    //void (*inv_function)(void *const);
    void * const obj;
    //void(* invoke_functee)(void *const);
    void * invoke_functee;
    void * bailee_requiree;
    //void * caller_requiree;
    //void * return_bailee_requiree;
public:


    template<typename bailee, typename bailee_return, typename bailee_require>	// _Convention see msdn: Argument Passing and Naming Conventions
    AgencyInput(bailee & _obj, bailee_return(bailee::*_inv)(bailee_require))
        : obj(& _obj) {

        //_____________________________________________________________________________________________
        try {
            if(!obj) {
                cerr<<endl<<"empty object!"<<endl;
                cin.get();
                //exit(1);
            }
        } catch(exception e) {
            throw e;
        }

        invoke_functee = (void *)0;
        invoke_functee = (void *) new _invoke_functor<bailee, bailee_return, bailee_require>(_inv);
        del = _release<bailee, bailee_return, bailee_require>;
        //bailee_requiree = (void *) & input_pamater;
        //caller_requiree = (void *) & caller_require;
        inv = _transmit_input<bailee, bailee_return, bailee_require>;

        //_____________________________________________________________________________________________

    }

    // not safety, [&&&^%^&##$%^%^^&%^$$#$#&^&**]
    template<typename bailee, typename bailee_return, typename bailee_require>
    void strategy(bailee_return (bailee::*_inv)(bailee_require)) {

        if(NULL != invoke_functee ) delete static_cast<_invoke_functor<bailee, bailee_return, bailee_require> *>(invoke_functee);
        _invoke_functor<bailee, bailee_return, bailee_require> *invoke = new _invoke_functor<bailee, bailee_return, bailee_require>(_inv);
        invoke_functee = (void *)invoke;
        del = _release<bailee, bailee_return, bailee_require>;
        //bailee_requiree = (void *) & input_pamater;
        //caller_requiree = (void *) & caller_require;
        inv = _transmit_input<bailee, bailee_return, bailee_require>;

    }
    template<typename bailee_require>
    void operator()(bailee_require & input_pamater) {	// he can return bailee_require !!!, if bailee_require is as same as object type? what happen?
        inv(obj, invoke_functee, &input_pamater);

        //return obj->inv(p);
        // if you cast bailee_require as other type not as same as in constructor give InvokeByPointer
        // , no problem, it can not pass compile->right idea
        // , only unify with static Wedge member in constructor->wrong idea
    }


    ~AgencyInput() {
        if(NULL != invoke_functee )
            del(invoke_functee);
    }

};

template<typename bailee, typename bailee_return, typename bailee_require>
struct _invoke_operator {
    bailee_return(bailee::*inv)(bailee_require);
    _invoke_operator( bailee_return(bailee::*_inv)(bailee_require) ) {
        inv = _inv;
    }
};

template<typename bailee, typename bailee_return>
struct _invoke_operator<bailee, bailee_return, void> {
    bailee_return(bailee::*inv)();
    _invoke_operator( bailee_return(bailee::*_inv)() ) {
        inv = _inv;
    }
};

template<typename bailee_return, typename bailee_require>
struct _invoke_operator_static {
    bailee_return(*inv)(bailee_require);
    _invoke_operator_static( bailee_return(*_inv)(bailee_require) ) {
        inv = _inv;
    }
};
template<typename bailee, typename bailee_return, typename bailee_require>
void _transmit_operator(void * const _bailee_returne, void * const _obj, void * const _invoke_obj, void const * _bailee_requiree)
{

    * (static_cast<bailee_return * const>(_bailee_returne)) = ((static_cast<bailee * const>(_obj)) ->* ((static_cast<_invoke_operator<bailee, bailee_return, bailee_require> * const>(_invoke_obj))->inv)) (*(static_cast<bailee_require const *>(_bailee_requiree)));

}

template<typename bailee, typename bailee_return>
void _transmit_operator_read(void * const _bailee_return, void * const _obj, void * const _invoke_obj)
{

    * (static_cast<bailee_return * const>(_bailee_return)) = ((static_cast<bailee * const>(_obj)) ->* ((static_cast<_invoke_operator<bailee, bailee_return, void> * const>(_invoke_obj))->inv)) ();

}
template<typename bailee_return, typename bailee_require>
void _transmit_operator_static(void * const _bailee_return, void * const _invoke_obj, void const * _bailee_requiree)
{

    * (static_cast<bailee_return * const>(_bailee_return)) = ((static_cast<_invoke_operator_static<bailee_return, bailee_require> * const>(_invoke_obj))->inv) (*(static_cast<bailee_require const *>(_bailee_requiree)));

}
template<typename bailee, typename bailee_return, typename bailee_require>
void _release_operator(void * _invoke_functee)
{
    if((void *)0 != _invoke_functee ) delete static_cast<_invoke_operator<bailee, bailee_return, bailee_require> *>(_invoke_functee);

}

template<typename bailee, typename bailee_return>
void _release_operator_read(void * _invoke_functee)
{
    if((void *)0 != _invoke_functee ) delete static_cast<_invoke_operator<bailee, bailee_return, void> *>(_invoke_functee);

}
template<typename bailee_return, typename bailee_require>
void _release_operator_static(void * _invoke_functee)
{
    if((void *)0 != _invoke_functee ) delete static_cast<_invoke_operator_static<bailee_return, bailee_require> *>(_invoke_functee);

}
class ioperator {

protected:

    void    (*inv)  (void * const _bailee_return, void * const _obj, void * const _invoke_obj, void const * _bailee_requiree);
    void    (*del)  (void *);
    void *  invoke_functee;
    void *  obj;
    void *  bailee_returne;

public:

    template<typename bailee, typename bailee_return, typename bailee_require>
    ioperator(bailee_return &_bailee_returne, bailee &_obj, bailee_return(bailee::*_inv)(bailee_require)) {
        invoke_functee =    (void *) new _invoke_operator<bailee, bailee_return, bailee_require>(_inv);
        obj =   (void *)&_obj;
        bailee_returne =    (void *)&_bailee_returne;
        del =   _release_operator<bailee, bailee_return, bailee_require>;
        inv =   _transmit_operator<bailee, bailee_return, bailee_require>;
    }

    template<typename bailee_require>
    void operator()(bailee_require &_bailee_requiree) {
        inv(bailee_returne, obj, invoke_functee, &_bailee_requiree);
    }

    ~ioperator() {
        if(NULL !=  invoke_functee)
            del(invoke_functee);
    }

};

class ioperator_read {
protected:
    void    (*inv)  (void * const _bailee_return, void * const _obj, void * const _invoke_obj);
    void    (*del)  (void *);
    void *  invoke_functee;
    void *  obj;
    void *  bailee_returne;
public:

    template<typename bailee, typename bailee_return>
    ioperator_read(bailee_return & _bailee_returne, bailee &_obj, bailee_return(bailee::*_inv)()) {
        invoke_functee =    (void *) new _invoke_operator<bailee, bailee_return, void>(_inv);
        obj =   &_obj;
        bailee_returne =    (void *)&_bailee_returne;
        del =   _release_operator_read<bailee, bailee_return>;
        inv =   _transmit_operator_read<bailee, bailee_return>;
    }

    void operator()() {
        inv(bailee_returne, obj, invoke_functee);
    }
    ~ioperator_read() {
        if(NULL !=  invoke_functee)
            del(invoke_functee);
    }
};

class ioperator_static {
protected:

    void    (*inv)  (void * const _bailee_return, void * const _invoke_obj, void const * _bailee_requiree);
    void    (*del)  (void *);
    void *  invoke_functee;

    template<typename bailee_return, typename bailee_require>
    ioperator_static(bailee_return(*_inv)(bailee_require)) {
        invoke_functee =    (void *) new _invoke_operator_static<bailee_return, bailee_require>(_inv);
        del =   _release_operator_static<bailee_return, bailee_require>;
        inv =   _transmit_operator_static<bailee_return, bailee_require>;
    }

    template<typename bailee_return, typename bailee_require>
    bailee_return operator()(bailee_require &_bailee_requiree) {
        bailee_return _bailee_return;
        inv(&_bailee_return, invoke_functee, &_bailee_requiree);
        return _bailee_return;
    }

    ~ioperator_static() {
        if(NULL !=  invoke_functee)
            del(invoke_functee);
    }

};




}

#endif // DELEGATE_H_INCLUDED
