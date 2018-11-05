#include<iostream>
#include "Judgment.h"

//#include "2013182043_±èÀÚ·á.h"
//#include "2013182044_±è±¸Á¶.h"
#include "2011144024_À¯´ë¼º.h"
//#include "2013180010_¹ÚÁ¤ÈÆ.h"
//#include "2017184037_È«Áø¼±.h"
//#include "2015182003_±ÇÈ£¹Î.h"
//#include "2013182024_¼Û¹Î¼ö.h"

using namespace std;

// ÇÑ Ä­ ¶è 3, 4 °ø·«

int main()
{
	Judgment judgement;

	judgement.SetYourFunc(AttackBlack_2011144024/*¼±°ø*/, DefenceBlack_2011144024, WhiteAttack_2011144024/*ÈÄ°ø*/, WhiteDefence_2011144024);
	judgement.GamePlay();

}