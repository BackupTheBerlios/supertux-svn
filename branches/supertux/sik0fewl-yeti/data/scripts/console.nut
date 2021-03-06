/**
 * This script is loaded into the console script interpreter.
 * You should define shortcuts and helper functions that are usefull for the
 * console here
 */

function flip()
{
	Level.flip_vertically();
}

function finish()
{
	Level.finish(true);
}

/**
 * Display a list of functions in the roottable (or in the table specified)
 */
function functions(...)
{
	local obj = this;
	if(vargc == 1)
		obj = vargv[0];
	if(::type(obj) == "instance")
		obj = obj.getclass()

	while(obj != null) {
		foreach(key, val in obj) {
			if(::type(val) == "function")
				println(key);
		}
		obj = obj.parent;
	}
}

