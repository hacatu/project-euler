import re

def combine(s, a, b):
	#print("combine({}, {}, {})".format(s, a, b))
	if len(a) == 0:
		return [s + b]
	if len(b) == 0:
		return [s + a]
	return combine(s + a[0], a[1:], b) + combine(s + b[0], a, b[1:])

def shorten(s):
	return re.sub(r'(.)\1+', r'\1', s)

def minimal(a):
	if len(a) < 2:
		return a
	a.sort(key = len)
	return [s for s in a if len(s) == len(a[0])]

#note: the passcode must begin with 7 since 7 is always in the beginning, so I've removed it.  Same for 0 and the end.  I also removed any 1 digit strings left after this.  Once the strings are deduplicated, there are 24 hints to process
logins = ["319", "68", "18", "69", "129", "62", "62", "689", "62", "318", "368", "629", "168", "16", "689", "16", "31", "36", "29", "316", "29", "29", "69", "29", "19", "68", "318", "389", "162", "289", "162", "18", "29", "319", "68", "89", "362", "319", "316", "29", "38", "319", "28", "16"]

logins = list(set(logins))

passcodes = [logins[0]]

for login in logins[1:]:
	_passcodes = []
	for passcode in passcodes:
		_passcodes += map(shorten, combine("", passcode, login))
	passcodes = list(set(minimal(_passcodes)))

print("7{0[0]}0".format(passcodes))

