
/// @file TclVarTrace.cc
/// @brief TclVarTrace の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/TclVarTrace.h"


BEGIN_NAMESPACE_YM_TCLPP

// コンストラクタ
TclVarTrace::TclVarTrace()
{
}

// デストラクタ
TclVarTrace::~TclVarTrace()
{
  unbind();
}

// 変数にバインドする．
// 既にバインドしている場合には無視する．
// 終了コードを返す．
int
TclVarTrace::bind(Tcl_Interp* interp,
		  const string& name1,
		  int flags)
{
  int result{TCL_OK};
  if ( !is_bound() ) {
    mName1 = name1;
    mName2 = "";
    mFlags = flags;
    // もともと flags に TCL_TRACE_UNSETS が含まれていなくても
    // それを足しておく．
    flags |= TCL_TRACE_UNSETS;
    const char* tmp_str{mName1.c_str()};
    result = Tcl_TraceVar(interp, tmp_str, flags,
			  vartrace_callback, (ClientData) this);
    if ( result == TCL_OK ) {
      set_interp(interp);
    }
  }
  return result;
}

// 変数にバインドする．
// 既にバインドしている場合には無視する．
// 終了コードを返す．
int
TclVarTrace::bind(Tcl_Interp* interp,
		  const string& name1,
		  const string& name2,
		  int flags)
{
  int result{TCL_OK};
  if ( !is_bound() ) {
    mName1 = name1;
    mName2 = name2;
    mFlags = flags;
    // もともと flags に TCL_TRACE_UNSETS が含まれていなくても
    // それを足しておく．
    flags |= TCL_TRACE_UNSETS;
    result = Tcl_TraceVar2(interp, mName1.c_str(), mName2.c_str(), flags,
			   vartrace_callback, (ClientData) this);
    if ( result == TCL_OK ) {
      set_interp(interp);
    }
  }
  return result;
}

// バインドを解く．
// 既にバインドがなければ何もしない．
// 常に成功する(はず)
void
TclVarTrace::unbind()
{
  // もともと mFlags に TCL_TRACE_UNSETS が含まれていなくても
  // それを足しておく．
  int flags{mFlags | TCL_TRACE_UNSETS};
  if ( is_bound() ) {
    if ( mName2 !=  "" ) {
      Tcl_UntraceVar2(interp(), mName1.c_str(), mName2.c_str(), flags,
		      vartrace_callback, (ClientData) this);
    }
    else {
      Tcl_UntraceVar(interp(), mName1.c_str(), flags,
		     vartrace_callback, (ClientData) this);
    }
    set_interp(nullptr);
  }
}

// トレースコールバックの入口
char*
TclVarTrace::vartrace_callback(ClientData clientData,
			       Tcl_Interp* interp,
			       const char* name1,
			       const char* name2,
			       int flags)
{
  // オブジェクトを得る．
  auto trace_obj{static_cast<TclVarTrace*>( clientData )};

  // 念のため trace_obj の持っているインタプリタと interp が一致
  // する事を確かめておく．
  ASSERT_COND( interp == trace_obj->interp() );

  char* result{nullptr};

  if ((flags & TCL_TRACE_UNSETS) == 0 ||
      (trace_obj->flags() & TCL_TRACE_UNSETS) == TCL_TRACE_UNSETS) {
    // 元の mFlags がどうであれ，TCL_TRACE_UNSETS を加えているので，
    // 本当のトレースコールバック関数を呼ぶべきかどうか判定する．
    result = trace_obj->vartrace_proc(name1, name2, flags);
  }

  if ( flags & TCL_TRACE_DESTROYED ) {
    // バインドを外す．
    trace_obj->set_interp(nullptr);
  }

  return result;
}

END_NAMESPACE_YM_TCLPP
