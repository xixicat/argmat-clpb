#include <cstdio>
#include <iostream>
#include <fstream>
#include <string.h>

#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"
#include "ArgumentProperty.hpp"
#include "AttackProperty.hpp"
#include "DungAF.hpp"
#include "Reasoner.hpp"
#include "GroundedReasoner.hpp"
#include "PreferredReasoner.hpp"

using namespace std;
using namespace argumatrix;


void showSet(const std::set<bitvector>& sa, DungAF& daf)
{
	std::set<bitvector>::iterator sa_itr;
	
	std::cout << "The argument set is: " << endl;
	for (sa_itr = sa.begin(); sa_itr != sa.end(); sa_itr++)
	{
		daf.outputBv( *sa_itr );
	}
}


void Test2_ElementsOfArgumentation()
{
	DungAF daf;

	Argument a = daf.addArgument("a");
	Argument b = daf.addArgument("b");
	Argument c = daf.addArgument("c");
	Argument d = daf.addArgument("d");
	Argument e = daf.addArgument("e");

	//daf.addAttack(a, b);
	daf.addAttack("a", "b");
	//cout << "1: " << daf.toString() << endl;
	daf.addAttack(c, b);
	daf.addAttack(c, d);
	//daf.addAttack(d, c);
	//cout << "2: " << daf.toString() << endl;
	daf.addAttack("d", "c");
	//cout << "3: " << daf.toString() << endl;
	daf.addAttack("d", e);
	//cout << "4: " << daf.toString() << endl;
	daf.addAttack(e, "e");
	//cout << "5: " << daf.toString() << endl;
	//daf.addAttack("g", "f");
	//cout << "6: " << daf.toString() << endl;

	PreferredReasoner pr(daf);

	daf.outputBvSet(pr.getBvExtensions());

	cout << "AF: " << daf.toString() << endl;

	daf.clear();

	cout << "cleared AF: " << daf.toString() << endl;
}


int main(int argc, char* argv[])
{
	DungAF daf;
	
	Argument a = daf.addArgument("A");
	Argument b = daf.addArgument("B");
	Argument c = daf.addArgument("C");
	Argument d = daf.addArgument("D");
	Argument e = daf.addArgument("E");
	Argument f = daf.addArgument("F");

	daf.addAttack(a, b);
	daf.addAttack(b, c);
	daf.addAttack(a, d);
	daf.addAttack(d, e);


	set<Argument> sa;
	sa.insert(a);
	sa.insert(b);
	sa.insert(d);

	bitvector bv = daf.set2bv(sa);

	cout<<" ****************************" << endl
		<< bv << endl;

	set<Argument> sa1 = daf.bv2set(bv);

	//daf.showSet(sa1);

	cout << "AF = " << daf.toString() << endl;

	cout << "UUUUU: " << daf.getArgumentProperty("A").label << endl;

	//cout << "U_Test 2: "<< (boost::none == daf.getArgumentProperty("BBB")) << endl;

	bitmatrix bm = daf.getAttackMatrix();

	bm.show();

	cout << endl << "Reasoner: " << endl;

	Reasoner reasoner(daf);

	bitvector bv_attacked = reasoner.getAttacked(bv);
	//bitvector bv_attackers = reasoner.getAttackers(bv);

	cout << "Attacked: " << bv_attacked << endl;
	//cout << "Attackers: " << bv_attackers << endl;

	cout << "The characteristic function of EmptySet is: " << reasoner.characteristic() << endl;

	cout << "bv is conflict-free? " << reasoner.is_conflict_free(bv) << endl;

	GroundedReasoner gr(daf);

	//gr.computeExtensions();
	set<bitvector> extenstion = gr.getBvExtensions();
	
	showSet(extenstion, daf);
	gr.printBvExts();
	size_type i = -1;

	cout << "size_type i=-1 :" << i << endl;

	PreferredReasoner pr(daf);
	pr.computeExtensions();

	pr.printBvExts();

	set<bitvector> pExt = pr.getBvExtensions();
	daf.outputBvSet(pExt);

	system("pause");

	DungAF daf2;
	daf2.addArgument("A");
	daf2.addArgument("B");
	daf2.addArgument("C");
	daf2.addArgument("D");
	daf2.addArgument("E");
	daf2.addArgument("F");
	
	daf2.addArgument("G");

	daf2.addAttack("A", "B");
	daf2.addAttack("B", "E");
	daf2.addAttack("C", "B");
	daf2.addAttack("D", "C");
	daf2.addAttack("E", "D");
	daf2.addAttack("E", "F");
	daf2.addAttack("F", "A");

	daf2.addAttack("C", "G");
	daf2.addAttack("G", "D");

	PreferredReasoner pr2(daf2);
	//daf2.outputBvSet(pr2.getBvExtensions());

	pr2.computeExtensions();

	system("pause");

	return 0;
}