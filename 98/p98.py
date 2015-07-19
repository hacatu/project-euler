from itertools import groupby, combinations, permutations
from math import sqrt

def letters(word):
	ret = [0]*26
	for letter in word:
		ret[ord(letter) - ord('A')] += 1
	return ret

def fst(t):
	return t[0]

def snd(t):
	return t[1]

def isSquare(n):
	r = int(sqrt(n))
	return r*r == n

def ltoi(digits):
	n = 0
	for digit in digits:
		n *= 10
		n += digit
		if n == 0:
			return 0
	return n

with open("words.txt", "r") as f:
	words = f.read().splitlines()
	words = [(letters(word), word) for word in words]
	words.sort(key = fst)
	aFamilies = groupby(words, key = fst)
	aFamilies = ([word for mask, word in family] for key, family in aFamilies)
	anagrams = [(sorted(set(family[0])), family) for family in aFamilies if len(family) > 1 and len(family[0]) >= 4]
	anagrams.sort(key = lambda family: -len(snd(family)[0]))
	maxSquare = 9216
	maxLen = 4
	for letters, family in anagrams:
		if len(family[0]) < maxLen:
			break
		for a, b in combinations(family, 2):
			print(a, b)
			for digits in permutations(range(10)[::-1], len(letters)):
				ltod = dict(zip(letters, digits))
				inta = ltoi(map(ltod.get, a))
				if inta == 0:
					continue
				if not isSquare(inta):
					continue
				intb = ltoi(map(ltod.get, b))
				if intb == 0:
					continue
				if not isSquare(intb):
					continue
				print("a and b square")
				print(digits)
				print(inta)
				print(intb)
				inta = max(inta, intb)
				if inta > maxSquare:
					maxSquare = inta
					maxLen = len(family[0])
	print(maxSquare)
