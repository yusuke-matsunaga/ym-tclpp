# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================

find_package (TCL REQUIRED)


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-base/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${TCL_INCLUDE_PATH}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (SOURCES
  src/TclBase.cc
  src/TclCmd.cc
  src/TclHandler.cc
  src/TclObj.cc
  src/TclObjMsgHandler.cc
  src/TclPopt.cc
  src/TclPoptGroup.h
  src/TclVarTrace.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
ym_add_object_library ( ym_tclpp
  ${SOURCES}
  )

set ( YM_LIB_DEPENDS ${YM_LIBS_DEPENDS} ${TCL_LIBRARY} )
