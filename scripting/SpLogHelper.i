%module(directors="1") story
%{
#include "scripting/SpLogHelper.hpp"
%}

%feature("director") SpLog;

%include "scripting/SpLogHelper.hpp"
