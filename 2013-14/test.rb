def b(num)
	num.to_s(2)
end

def test(d)
	if d<500
		p b d
	elsif 512&d>0
		p "512: "+b(d)
		return 512^d
	elsif 1024&d>0
		p "1024: "+b(d)
		return 1024^d
	elsif 2048&d>0
		p "2048: "+b(d)
		return 2048^d
	elsif 4096&d>0
		p "4096: "+b(d)
		return 4096^d
	end
end
