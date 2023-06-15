//Header file for function to define particle generator

#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "run.hh"
#include "event.hh"
#include "stepping.hh"

//inherited from class G4VUserActionInitialization
class MyActionInitialization : public G4VUserActionInitialization
{
public:
	//construct and destruct action
	MyActionInitialization();
	~MyActionInitialization();

	//main function that runs particle gun
	//and controls stepping
	virtual void Build() const;
};

#endif
