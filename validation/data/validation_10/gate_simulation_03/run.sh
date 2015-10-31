TEST_NUMBER=15
NB_RUN=10

rm -rf run_*
rm -f benchmarkVRTCT_*.dat
rm -f benchmarkVRTCT.root
rm -f timing2.dat


FIRST=`echo "$TEST_NUMBER * $NB_RUN" |bc`
LAST=$(expr $FIRST + $NB_RUN)



for (( i = $FIRST; i < $LAST; i++ ))
do
    # Create first seed
    SEED1=`echo $[ 1 + $[ RANDOM % 100000000 ]]`

    # Create second seed
    SEED2=`echo $[ 1 + $[ RANDOM % 100000000 ]]`

    # Create output.mac file
    cat ../gate_simulation/output.start > output.mac
    echo "/gate/random/setEngineSeed $SEED1"        >> output.mac
    echo "/gate/output/imageCT/setStartSeed $SEED2" >> output.mac

    echo Start Run $i
    echo "********************************************************************************" >> timing2.dat
    echo Start >> timing2.dat
    echo Run $i >> timing2.dat
    date >> timing2.dat

    /usr/local/share/gate_v6.2/bin/Gate < ../gate_simulation/benchmarkVRTCT.mac 2>/dev/null >/dev/null
    date >> timing2.dat
    echo Stop >> timing2.dat
    echo "********************************************************************************" >> timing2.dat
    echo >> timing2.dat
    echo >> timing2.dat
    echo >> timing2.dat
    echo >> timing2.dat
    echo >> timing2.dat

    mkdir ../gate_simulation/run_$i
    mv benchmarkVRTCT*.root ../gate_simulation/run_$i
    mv output.mac ../gate_simulation/run_$i
done

xclock

