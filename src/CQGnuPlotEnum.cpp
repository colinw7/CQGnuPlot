#include <CQGnuPlotEnum.h>
#include <CEnumConv.h>

namespace Conv {

typedef CEnumConv<CGnuPlotTypes::SymbolType, CQGnuPlotEnum::SymbolType> SymbolConv;

SymbolConv symbolConv({
  { CGnuPlotTypes::SymbolType::NONE               , CQGnuPlotEnum::SymbolNone            },
  { CGnuPlotTypes::SymbolType::POINT              , CQGnuPlotEnum::SymbolPoint           },
  { CGnuPlotTypes::SymbolType::PLUS               , CQGnuPlotEnum::SymbolPlus            },
  { CGnuPlotTypes::SymbolType::CROSS              , CQGnuPlotEnum::SymbolCross           },
  { CGnuPlotTypes::SymbolType::STAR               , CQGnuPlotEnum::SymbolStar            },
  { CGnuPlotTypes::SymbolType::BOX                , CQGnuPlotEnum::SymbolBox             },
  { CGnuPlotTypes::SymbolType::FILLED_BOX         , CQGnuPlotEnum::SymbolFilledBox       },
  { CGnuPlotTypes::SymbolType::CIRCLE             , CQGnuPlotEnum::SymbolCircle          },
  { CGnuPlotTypes::SymbolType::FILLED_CIRCLE      , CQGnuPlotEnum::SymbolFilledCircle    },
  { CGnuPlotTypes::SymbolType::TRIANGLE           , CQGnuPlotEnum::SymbolTriangle        },
  { CGnuPlotTypes::SymbolType::FILLED_TRIANGLE    , CQGnuPlotEnum::SymbolFilledTriangle  },
  { CGnuPlotTypes::SymbolType::INV_TRIANGLE       , CQGnuPlotEnum::SymbolITriangle       },
  { CGnuPlotTypes::SymbolType::FILLED_INV_TRIANGLE, CQGnuPlotEnum::SymbolFilledITriangle },
  { CGnuPlotTypes::SymbolType::DIAMOND            , CQGnuPlotEnum::SymbolDiamond         },
  { CGnuPlotTypes::SymbolType::FILLED_DIAMOND     , CQGnuPlotEnum::SymbolFilledDiamond   }
});

}

//---

CQGnuPlotEnum::SymbolType
CQGnuPlotEnum::
symbolConv(const CGnuPlotTypes::SymbolType &type)
{
  if (int(type) >= 0 && int(type) <= int(CGnuPlotTypes::SymbolType::LAST))
    return Conv::symbolConv.conv(type);
  else
    return CQGnuPlotEnum::SymbolNone;
}

CGnuPlotTypes::SymbolType
CQGnuPlotEnum::
symbolConv(const CQGnuPlotEnum::SymbolType &type)
{
  return Conv::symbolConv.conv(type);
}
