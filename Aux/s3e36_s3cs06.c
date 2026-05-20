extern node_t*
findRingOrigin( node_t* H, size_t* n, size_t* N )
{
	node_t*			slow = H;
	node_t*			fast = H;

	*n = *N = 0;

	//
	// Find X if it exists
	//
	while ( fast->next )
	{
		(*N)++;
		slow = slow->next;
		fast = fast->next->next;

		if ( slow == fast ) // X marks the spot
		{
			break;
		}
	}

	//
	// Return null if there is no ring
	//
	if ( slow != fast )
	{
		*N = 0;
		return NULL;
	}

	//
	// keep fast at X, put slow into H
	//
	slow = H;

	//
	// Move both racers at the same speed
	//
	while ( slow != fast )
	{
		(*n)++;
		slow = slow->next;
		fast = fast->next;
	}

	return slow; // or fast, it does not matter
}
