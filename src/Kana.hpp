#pragma once

static const int nKana = 45;
static const int nDiac = 25;
static const int nTotal = nKana + nDiac;

static const char* romaji[] {
"a","ka","sa","ta","na","ha","ma","ya","ra","wa",
"i","ki","shi","chi","ni","hi","mi","ri",
"u","ku","su","tsu","nu","fu","mu","yu","ru",
"e","ke","se","te","ne","he","me","re",
"o","ko","so","to","no","ho","mo","yo","ro","wo",

"ga","za","da","ba","pa",
"gi","ji","dji","bi","pi",
"gu","zu","dzu","bu","pu",
"ge","ze","de","be","pe",
"go","zo","do","bo","po"};

static const wchar_t kana[] {
L'あ',L'か',L'さ',L'た',L'な',L'は',L'ま',L'や',L'ら',L'わ',
L'い',L'き',L'し',L'ち',L'に',L'ひ',L'み',		L'り',
L'う',L'く',L'す',L'つ',L'ぬ',L'ふ',L'む',L'ゆ',L'る',
L'え',L'け',L'せ',L'て',L'ね',L'へ',L'め',		L'れ',
L'お',L'こ',L'そ',L'と',L'の',L'ほ',L'も',L'よ',L'ろ',L'を',

L'が',L'ざ',L'だ',L'ば',L'ぱ',
L'ぎ',L'じ',L'ぢ',L'び',L'ぴ',
L'ぐ',L'ず',L'づ',L'ぶ',L'ぷ',
L'げ',L'ぜ',L'で',L'べ',L'ぺ',
L'ご',L'ぞ',L'ど',L'ぼ',L'ぽ',

L'ア',L'カ',L'サ',L'タ',L'ナ',L'ハ',L'マ',L'ヤ',L'ラ',L'ワ',
L'イ',L'キ',L'シ',L'チ',L'ニ',L'ヒ',L'ミ',		L'リ',
L'ウ',L'ク',L'ス',L'ツ',L'ヌ',L'フ',L'ム',L'ユ',L'ル',
L'エ',L'ケ',L'セ',L'テ',L'ネ',L'ヘ',L'メ',		L'レ',
L'オ',L'コ',L'ソ',L'ト',L'ノ',L'ホ',L'モ',L'ヨ',L'ロ',L'ヲ',

L'ガ',L'ザ',L'ダ',L'バ',L'パ',
L'ギ',L'ジ',L'ヂ',L'ビ',L'ピ',
L'グ',L'ズ',L'ヅ',L'ブ',L'プ',
L'ゲ',L'ゼ',L'デ',L'ベ',L'ペ',
L'ゴ',L'ゾ',L'ド',L'ボ',L'ポ'};
