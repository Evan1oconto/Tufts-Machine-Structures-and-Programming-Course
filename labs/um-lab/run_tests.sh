#! /bin/sh

testFiles=$(ls *.um)
for testFile in $testFiles ; do
  testName=$(echo $testFile | sed -E 's/(.*).um/\1/')

  echo $testName

  if [-f "${testName}.0" ] ; then
    input="${testName}.0"
  else  
    input="/dev/null"
  fi

  if [ -f "${testName}.1" ] ; then
    cat $input | ../UM $testFile > "${testName}.out"
    diff "${testName}.out" "${testName}.1"
  else
    cat $input | ../UM $testFile
  fi
  
done
