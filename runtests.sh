#The original tests are available in https://www.l2f.inesc-id.pt/~david/w/pt/Compiladores/Projecto_de_Compiladores/Testes_Autom%C3%A1ticos_2017-2018
passed=0
percs=0
for letter in {A,B,C,D,E,F,J,K,L,O,P,Q,R}; do
	ltrpass=0
	ltrtotal=0
	for file in ${letter}*.gr8; do  #run this on your test folder, obviously.
		echo ${file}
		file=${file%.gr8}
		../gr8/gr8 --target asm ${file}.gr8 && #change ../gr8/gr8 to wherever your gr8 executable is
		yasm -felf32 ${file}.asm &&
		#this assumes your rts is installed in /usr/something, add -L$HOME/compiladores/root/usr/lib before -lrts otherwise
		ld -m elf_i386 -o ${file}.exe ${file}.o -lrts &&
		./${file}.exe > ${file}.myout
		#uncomment below if you want to see your output beside the diff
		#cat ${file}.myout
		#echo ""
		#comment below if you want a different diff
		#DIFF=$(diff ${file}.myout expected/${file}.out)
		DIFF=$(diff <( tr -d ' \t\n' <${file}.myout ) <( tr -d ' \t\n' <expected/${file}.out))
		if [ "$DIFF" == "" ]
		then
			let "ltrpass=ltrpass+1"
			echo "OK!"
		else
			echo ""
			echo "$DIFF"
			echo "TEST FAILED!"
			echo ""
		fi
		let "ltrtotal=ltrtotal+1"
		#uncomment the below files if you want to see the test code below the diff.
		#echo ""
		#echo "test code:"
		#cat ${file}.gr8
	done
	percltr=$((ltrpass*100/ltrtotal))
	echo ""
	echo "${letter}:	${ltrpass}/${ltrtotal}	tests passed.	Percentage:	${percltr}%"
	echo ""
	percs=$((percs+percltr))
	passed=$((passed+ltrpass))
done
total=$((percs/13))
echo "TOTAL:	${passed}/107	tests passed.	Percentage:	${total}%"
rm *.o *.myout *.exe *.asm   #<-uncomment if you want to clean up your garbage afterwards
