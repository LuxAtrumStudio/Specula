static std::string colors[17] = {
    "Red",       "Pink",  "Purple", "Deep Purple", "Indigo",     "Blue",
    "Pale Blue", "Cyan",  "Teal",   "Green",       "Pale Green", "Lime",
    "Yellow",    "Amber", "Orange", "Deep Orange", "Brown"};
static std::map<std::string, ImVec4> isotope = {
    {"light_Red", ImVec4(0.99609375, 0.53515625, 0.37890625, 1.0)},
    {"Red", ImVec4(0.953125, 0.26171875, 0.2109375, 1.0)},
    {"dark_Red", ImVec4(0.7265625, 0.0, 0.05078125, 1.0)},
    {"light_Pink", ImVec4(0.99609375, 0.375, 0.5625, 1.0)},
    {"Pink", ImVec4(0.91015625, 0.1171875, 0.38671875, 1.0)},
    {"dark_Pink", ImVec4(0.6875, 0.0, 0.2265625, 1.0)},
    {"light_Purple", ImVec4(0.8125, 0.359375, 0.88671875, 1.0)},
    {"Purple", ImVec4(0.609375, 0.15234375, 0.6875, 1.0)},
    {"dark_Purple", ImVec4(0.4140625, 0.0, 0.5, 1.0)},
    {"light_Deep Purple", ImVec4(0.6015625, 0.40234375, 0.9140625, 1.0)},
    {"Deep Purple", ImVec4(0.40234375, 0.2265625, 0.71484375, 1.0)},
    {"dark_Deep Purple", ImVec4(0.1953125, 0.04296875, 0.5234375, 1.0)},
    {"light_Indigo", ImVec4(0.45703125, 0.48828125, 0.90625, 1.0)},
    {"Indigo", ImVec4(0.24609375, 0.31640625, 0.70703125, 1.0)},
    {"dark_Indigo", ImVec4(0.0, 0.16015625, 0.515625, 1.0)},
    {"light_Blue", ImVec4(0.4296875, 0.7734375, 0.99609375, 1.0)},
    {"Blue", ImVec4(0.12890625, 0.5859375, 0.94921875, 1.0)},
    {"dark_Blue", ImVec4(0.0, 0.41015625, 0.75, 1.0)},
    {"light_Pale Blue", ImVec4(0.40234375, 0.8515625, 0.99609375, 1.0)},
    {"Pale Blue", ImVec4(0.01171875, 0.66015625, 0.953125, 1.0)},
    {"dark_Pale Blue", ImVec4(0.0, 0.4765625, 0.75390625, 1.0)},
    {"light_Cyan", ImVec4(0.3828125, 0.93359375, 0.99609375, 1.0)},
    {"Cyan", ImVec4(0.0, 0.734375, 0.828125, 1.0)},
    {"dark_Cyan", ImVec4(0.0, 0.54296875, 0.63671875, 1.0)},
    {"light_Teal", ImVec4(0.3203125, 0.77734375, 0.71875, 1.0)},
    {"Teal", ImVec4(0.0, 0.5859375, 0.53125, 1.0)},
    {"dark_Teal", ImVec4(0.0, 0.40234375, 0.35546875, 1.0)},
    {"light_Green", ImVec4(0.5, 0.8828125, 0.4921875, 1.0)},
    {"Green", ImVec4(0.296875, 0.68359375, 0.3125, 1.0)},
    {"dark_Green", ImVec4(0.03125, 0.49609375, 0.13671875, 1.0)},
    {"light_Pale Green", ImVec4(0.7421875, 0.9609375, 0.4765625, 1.0)},
    {"Pale Green", ImVec4(0.54296875, 0.76171875, 0.2890625, 1.0)},
    {"dark_Pale Green", ImVec4(0.3515625, 0.5703125, 0.0859375, 1.0)},
    {"light_Lime", ImVec4(0.99609375, 0.99609375, 0.4296875, 1.0)},
    {"Lime", ImVec4(0.80078125, 0.859375, 0.22265625, 1.0)},
    {"dark_Lime", ImVec4(0.59765625, 0.6640625, 0.0, 1.0)},
    {"light_Yellow", ImVec4(0.99609375, 0.99609375, 0.4453125, 1.0)},
    {"Yellow", ImVec4(0.99609375, 0.91796875, 0.23046875, 1.0)},
    {"dark_Yellow", ImVec4(0.78125, 0.72265625, 0.0, 1.0)},
    {"light_Amber", ImVec4(0.99609375, 0.94921875, 0.3125, 1.0)},
    {"Amber", ImVec4(0.99609375, 0.75390625, 0.02734375, 1.0)},
    {"dark_Amber", ImVec4(0.77734375, 0.56640625, 0.0, 1.0)},
    {"light_Orange", ImVec4(0.99609375, 0.78515625, 0.27734375, 1.0)},
    {"Orange", ImVec4(0.99609375, 0.59375, 0.0, 1.0)},
    {"dark_Orange", ImVec4(0.7734375, 0.41015625, 0.0, 1.0)},
    {"light_Deep Orange", ImVec4(0.87890625, 0.5390625, 0.3125, 1.0)},
    {"Deep Orange", ImVec4(0.87890625, 0.33984375, 0.1328125, 1.0)},
    {"dark_Deep Orange", ImVec4(0.765625, 0.109375, 0.0, 1.0)},
    {"light_Brown", ImVec4(0.66015625, 0.5078125, 0.453125, 1.0)},
    {"Brown", ImVec4(0.47265625, 0.33203125, 0.28125, 1.0)},
    {"dark_Brown", ImVec4(0.29296875, 0.171875, 0.125, 1.0)},
    {"grey1", ImVec4(0.921875, 0.93359375, 0.94140625, 1.0)},
    {"grey2", ImVec4(0.80859375, 0.84375, 0.859375, 1.0)},
    {"grey3", ImVec4(0.6875, 0.7421875, 0.76953125, 1.0)},
    {"grey4", ImVec4(0.5625, 0.640625, 0.6796875, 1.0)},
    {"grey5", ImVec4(0.46875, 0.5625, 0.609375, 1.0)},
    {"grey6", ImVec4(0.375, 0.48828125, 0.54296875, 1.0)},
    {"grey7", ImVec4(0.328125, 0.4296875, 0.4765625, 1.0)},
    {"grey8", ImVec4(0.26953125, 0.3515625, 0.390625, 1.0)},
    {"grey9", ImVec4(0.21484375, 0.27734375, 0.30859375, 1.0)},
    {"grey10", ImVec4(0.1484375, 0.1953125, 0.21875, 1.0)}};
