#pragma once

#define FUNC_NOT(x) [](auto const& e){return !x(e);}
