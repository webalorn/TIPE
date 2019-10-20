#!/bin/bash

# Attention, ceci ne fonctionnera pas si la macro ConvertToPDF n'a pas été installée dans OpenOffice.org !
# Consultez LISEZMOI.TXT pour plus d'informations.

echo $1
echo $(readlink -f $1)
soffice -invisible "macro:///ConvertToPDF.Module1.ConvertToPDF("$(readlink -f $1)")"
while [ ! -f $(echo $1 |  cut -d'.' -f1).pdf ]; do
  echo "Waiting for soffice to finish..."
  sleep 1
done
pdfcrop $(echo $1 |  cut -d'.' -f1).pdf
mv $(echo $1 |  cut -d'.' -f1)-crop.pdf $(echo $1 |  cut -d'.' -f1).pdf

