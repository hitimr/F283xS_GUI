#include "cli.h"


///////////////////////////////////////////////////////////////////////////////
//
//	CLI Methods
//
///////////////////////////////////////////////////////////////////////////////

Cli::Cli(F28377S_Device * new_hDevce)
{
	hDevice = new_hDevce;

	connect(this, SIGNAL(editingFinished()), this, SLOT(parse()));
}

///////////////////////////////////////////////////////////////////////////////


Cli::~Cli()
{
}

void Cli::parse()
{
	QString input = text();	// load input
	clear();	// clear line

	bool isNumberical;
	int numerical_command = input.toInt(&isNumberical, 16);
	if (isNumberical)
	{
		hDevice->send_command(numerical_command);
	}
}
