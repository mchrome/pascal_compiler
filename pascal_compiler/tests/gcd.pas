program gcd;

type 
	Bptr = boolean;

var a,b,c, t:integer;
	inpt, outpt: string;
	d: Bptr;
	e: boolean;
	f,h: real;


{
	this is a comment
}


function GCD(m,n:integer):integer;

	type 
		w = boolean;

	var ee:integer;


	function FFFF(m,n:integer):integer;

		type 
			w1 = boolean;

		var ee1:integer;

		begin
			while m<>n do
			if m>n then m:=m-n else n:=n-m;
			GCD:=m;
		end;


	begin
		while m<>n do
		if m>n then m:=m-n else n:=n-m;
		GCD:=m;
	end;

begin
	inpt := 'Input 3 numbers';

	e:= true;
	d := e;

	f := 123.4567;


	
	a := (a+b)*c - d/ ( GCD(64,18));

	if (a = 30) and (b >= 15) or (c <= 17) xor (t <> 20) and not false then outpt := 'Fine';

	case a = 30  of
		1 : outpt := 'Fine';
		2: outpt := 'Fine';
	end;

	case a  of
		'one' : f := 1;
		'two': f := 2;
	end;

end.
