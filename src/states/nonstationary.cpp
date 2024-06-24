#include "nonstationary.h"

Nonstationary::Nonstationary() : table_{{{{2,12}},{{144,5}},{{7,5}},{{25,14}},{{54,5}},{{16,29}},{{26,140}},{{169,5}},{{16,24}},{{84,4}},{{16,37}},{{10,98}},{{10,212}},{{45,37}},{{201,0}},{{28,202}},{{32,23}},{{183,136}},{{6,217}},{{188,38}},{{8,37}},{{21,69}},{{8,61}},{{56,13}},{{22,37}},{{27,66}},{{85,140}},{{35,41}},{{107,199}},{{8,91}},{{102,217}},{{100,17}},{{104,23}},{{94,5}},{{96,47}},{{43,176}},{{46,48}},{{22,39}},{{107,51}},{{24,51}},{{251,248}},{{25,1}},{{54,34}},{{53,148}},{{70,58}},{{86,23}},{{22,61}},{{70,23}},{{36,63}},{{61,63}},{{7,5}},{{62,65}},{{90,216}},{{67,105}},{{68,34}},{{150,5}},{{96,73}},{{70,47}},{{72,74}},{{73,75}},{{46,63}},{{74,49}},{{20,89}},{{36,79}},{{20,138}},{{62,81}},{{170,14}},{{82,69}},{{84,34}},{{21,233}},{{96,72}},{{10,212}},{{44,59}},{{44,59}},{{58,76}},{{58,76}},{{75,61}},{{60,49}},{{22,39}},{{77,89}},{{19,65}},{{62,91}},{{90,217}},{{150,185}},{{94,34}},{{92,127}},{{96,23}},{{167,207}},{{146,147}},{{77,99}},{{93,102}},{{11,101}},{{97,105}},{{103,249}},{{104,34}},{{19,65}},{{106,57}},{{122,123}},{{22,39}},{{48,109}},{{0,203}},{{11,111}},{{3,83}},{{108,112}},{{114,34}},{{25,245}},{{116,34}},{{19,39}},{{113,230}},{{48,119}},{{166,188}},{{62,121}},{{140,96}},{{117,27}},{{124,4}},{{194,231}},{{42,34}},{{141,127}},{{164,179}},{{63,129}},{{235,204}},{{107,131}},{{125,127}},{{110,217}},{{134,4}},{{132,133}},{{18,245}},{{201,14}},{{62,175}},{{48,139}},{{128,183}},{{11,151}},{{137,30}},{{35,204}},{{135,4}},{{144,4}},{{175,149}},{{142,217}},{{62,183}},{{48,51}},{{201,1}},{{189,171}},{{145,133}},{{95,178}},{{150,33}},{{152,153}},{{95,195}},{{167,179}},{{35,66}},{{64,172}},{{154,9}},{{107,155}},{{156,157}},{{27,239}},{{158,9}},{{38,159}},{{160,161}},{{67,194}},{{162,33}},{{80,143}},{{163,185}},{{201,249}},{{166,9}},{{165,52}},{{0,231}},{{168,140}},{{110,188}},{{100,207}},{{173,176}},{{177,33}},{{174,188}},{{21,0}},{{151,192}},{{181,69}},{{27,154}},{{252,138}},{{66,33}},{{189,9}},{{228,143}},{{31,40}},{{190,90}},{{184,185}},{{188,121}},{{63,187}},{{186,148}},{{141,52}},{{193,83}},{{128,191}},{{114,182}},{{205,220}},{{197,52}},{{128,195}},{{146,212}},{{196,52}},{{100,215}},{{130,199}},{{198,133}},{{236,239}},{{206,233}},{{252,203}},{{100,203}},{{3,66}},{{146,254}},{{91,207}},{{185,194}},{{229,1}},{{209,230}},{{95,211}},{{100,17}},{{214,216}},{{100,17}},{{130,215}},{{64,225}},{{201,204}},{{218,239}},{{146,219}},{{166,230}},{{249,1}},{{222,204}},{{200,223}},{{134,1}},{{167,215}},{{226,174}},{{131,227}},{{221,247}},{{64,237}},{{232,69}},{{87,231}},{{19,65}},{{238,220}},{{35,12}},{{200,17}},{{180,173}},{{67,217}},{{28,101}},{{197,213}},{{240,241}},{{252,253}},{{242,220}},{{108,164}},{{243,244}},{{201,164}},{{246,41}},{{120,213}},{{201,52}},{{154,1}},{{126,55}},{{115,118}},{{115,118}},{{25,216}},{{38,195}},{{167,215}},{{20,138}},{{64,15}},{{28,88}},{{14,17}}}} {}

int Nonstationary::Next(int state, int bit) const {
  return table_[state][bit];
}

float Nonstationary::InitProbability(int state) const {
  return 0.5;
}
