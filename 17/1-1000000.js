function i2s_g(i){
	const teens = ["", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "ninteen"];
	const nties = ["", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"];
	var t;
	if(i < 20){
		return teens[i];
	}
	t = nties[~~(i/10)%10] + (i%10 ? "-" + teens[i%10] : "");
	if(i < 100){
		return t;
	}
	return teens[~~(i/100)] + (i%100 ? " hundred and " + t : " hundred");
}

function i2s(i){
	if(!i){
		return "zero";
	}
}

function i2s(i){
	var t;
	if(!i){
		return "";
	}else if(i < 13){
		return ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve"][i - 1];
	}else if(i < 16)
		return ["thir", "four", "fif"][i-13] + "teen";
	}else if(i < 20)
		return i2s(i%10) + "teen";
	}else if(i < 60){
		t = i2s(i%10);
		if(t){
			t = "-" + t;
		}
		return ["twen", "thir", "for","fif"][~~(i/10) - 2] + "ty" + t;
	}else if(i < 100){
		t = i2s(i%10);
		if(t){
			t = "-" + t;
		}
		return i2s(~~(i/10)) + "ty" + t;
	}else if(i < 1000){
		t = i2s(i%100);
		if(t){
			t = " and " + t;
		}
		return i2s(~~(i/100)) + " hundred" + t;
	}else if(i == 1000){
		return "one thousand";
	}else return NaN;
}
alert(i2s(prompt("ASDF:")));
