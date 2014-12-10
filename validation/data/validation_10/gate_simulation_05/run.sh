TEST_NUMBER=5
NB_RUN=10

rm -rf run_*
rm -f benchmarkVRTCT_*.dat
rm -f benchmarkVRTCT.root
rm -f timing.dat


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
    echo "********************************************************************************" >> timing.dat
    echo Start >> timing.dat
    echo Run $i >> timing.dat
    date >> timing.dat

    /usr/local/share/gate_v7.0/bin/Gate < ../gate_simulation/benchmarkVRTCT.mac 2>/dev/null >/dev/null
    date >> timing.dat
    echo Stop >> timing.dat
    echo "********************************************************************************" >> timing.dat
    echo >> timing.dat
    echo >> timing.dat
    echo >> timing.dat
    echo >> timing.dat
    echo >> timing.dat

    mkdir ../gate_simulation/run_$i
    mv benchmarkVRTCT_*.dat ../gate_simulation/run_$i
    mv benchmarkVRTCT*.root ../gate_simulation/run_$i
    mv output.mac ../gate_simulation/run_$i
done

xclock

