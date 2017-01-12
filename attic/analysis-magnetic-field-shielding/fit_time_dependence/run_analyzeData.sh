#!/bin/zsh

# DataFile_160811_163122.txt : 10L 20A
# DataFile_160808_132712.txt : 10L 8A
# DataFile_160816_183339.txt : 45L 23A

while read flayer fcurrent ffile
do
  if [[ -z "${ffile// }" ]]; then
    continue
  fi

  echo "_______________________________"
  echo "Layer is     : $flayer"
  echo "Current is   : $fcurrent"
  echo "File is      : $ffile"

  root -b -q analyzeTimeDependence.C\(\"data/${ffile}\"\)

  fprefix=${ffile/.txt/}

  plotlist=('RawData.png' 'SmoothData.png' 'FitResult.png' 'Chisquare.png' 'Contour.png' 'Residual.png')

  for plotsuffix in $plotlist; do

    plotname=""${fprefix}_L${flayer}_A${fcurrent}_${plotsuffix}""

    mv new_${plotsuffix} plots/${plotname}

  done

done < data/datalog_raphael.txt
