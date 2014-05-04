/*******************************************************************************
*  file    : qtxmlrpcconfig.hpp
*  created : 04.05.2014
*  author  :
*******************************************************************************/

#ifndef QTXMLRPCCONFIG_HPP
#define QTXMLRPCCONFIG_HPP

#include <QtGlobal>


#ifdef QXMLRPC_SHARED
#  define QXMLRPC_DECL Q_DECL_EXPORT
#else
#  define QXMLRPC_DECL Q_DECL_IMPORT
#endif

#endif // QTXMLRPCCONFIG_HPP
