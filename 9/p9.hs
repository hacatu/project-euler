let prod ([(a,b,c)]) = a*b*c in prod [(a,b,1000-a-b) | a<-[1..1000], b<-[1..a], a+b<1000, a^2+b^2 == (1000-a-b)^2]
