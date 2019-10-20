LATEX= latex
PDFLATEX= pdflatex
LATEX2HTML= latex2html -ascii_mode -no_tex_defs -no_navigation -no_auto_link -no_subdir -html_version 4.0,unicode -split 0 -info 0
TIDY= -tidy -utf8 -i -wrap 68 -asxhtml
HTML2TXT= lynx -nomargins -dump
ISO2UTF= iconv -f ISO-8859-15 -t UTF-8
CSV2LATEX= csv2latex-0.13/csv2latex

GRAPHS= all_tlds_vs_rand.tex all_tlds_to_all_tlds.custom.tex dist_vs_gdist_before.tex dist_vs_gdist_after.tex
GRAPHS_S= all_tlds_vs_rand.csv all_tlds_to_all_tlds.csv dist_vs_gdist_before.csv dist_vs_gdist_after.csv
IMAGES= images/r2002_at.pdf images/r2002_rnd_at.pdf images/r1822_ch.pdf images/r1822_ch_zoom.pdf images/r1822_keys_colors.pdf images/r1822_center_colors.pdf images/r1822_ethereal_convent.pdf images/r1822_byu_edu.pdf images/r1822_byu_edu_rand.pdf images/r1822_sony.pdf images/r1822_uni_potsdam_de.pdf images/r2014_accel.pdf images/r2014_all_colors.pdf images/r2014_ethereal_convent.pdf
IMAGES_S= images/r2002_at.png images/r2002_rnd_at.png images/r1822_ch.png images/r1822_ch_zoom.png images/r1822_keys_colors.png images/r1822_center_colors.png images/r1822_ethereal_convent.png images/r1822_byu_edu.png images/r1822_byu_edu_rand.png images/r1822_sony.png images/r1822_uni_potsdam_de.png images/r2014_accel.png images/r2014_all_colors.png images/r2014_ethereal_convent.png
CODE= distances.c events.c force.c graphics.c load.c main.c marks.c misc.c structures.c main.h
SCHEMAS= schemas/sch_alice_bob.pdf schemas/sch_alice_bob_mallory.pdf schemas/sch_alice_carol_bob.pdf schemas/sch_alice_carol_dave_zoe.pdf
SCHEMAS_S= schemas/sch_alice_bob.odg schemas/sch_alice_bob_mallory.odg schemas/sch_alice_carol_bob.odg schemas/sch_alice_carol_dave_zoe.odg
SOURCES= $(CODE) $(GRAPHS_S) $(IMAGES_S) $(SCHEMAS_S) fiche_synoptique.txt description.tex figs.tex TODO COPYING SOURCES GPL LISEZMOI.TXT convert_to_pdf.macro odg2pdf.sh fonts SDL_Input SDL_Input_32 csv2latex-0.13 Makefile

all: tipe description.pdf figs.pdf zip

tipe: $(CODE)
	gcc -o tipe -Wextra $(CODE) -lm -lSDL -lSDL_ttf -LSDL_Input -lSDL_Input -LSDL_Input/SDL_Input_TTF -lSDL_Input_TTF -O3
# -pg

%.pdf: %.odg
	./odg2pdf.sh $*.odg

%.sort.csv: %.csv
	cat $*.csv | head -1 > header
	sort -t";" -gr -k 2 $*.csv | head -42 > data
	cat header data > $*.sort.csv

# Disable sorting when it isn't needed.
%.custom.tex: %.csv
	$(CSV2LATEX) --reduce 0.8 --lines 45 --nohead --separator s --position c $*.csv > $*.custom.tex

%.tex: %.sort.csv
	./csv2latex-0.13/csv2latex --reduce 0.8 --lines 45 --nohead --separator s --position c $*.sort.csv > $*.tex

%.pdf: %.tex $(GRAPHS) $(IMAGES) $(CODE) $(SCHEMAS)
	pdflatex $*.tex
	pdflatex $*.tex
	pdflatex $*.tex

%.pdf: %.png
	convert $< $@

%.nomath.tex: %.tex
	cat $*.tex | tr -d $$ > $*.nomath.tex

%.only.html: %.nomath.tex
	$(LATEX2HTML) $*.nomath.tex
	mv $*.nomath.html $*.only.html

%.html: %.only.html
	cat $*.only.html | perl -0777 -pi -e "s/<ADDRESS>.*<\/ADDRESS>//sg" | perl -pi -e "s/tex2html_deferred//sg" | perl -pi -e "s/<HR>//sg" | perl -pi -e "s/(\&\#\d\d\d)/\$\1;/sg" | perl -0777 -pi -e "s/\&\#171;\s*/\&\#171;\&nbsp;/g" | perl -0777 -pi -e "s/\s*\&\#187;/\&nbsp;\&\#187;/g" | perl -0777 -pi -e "s/\&\#187;\s*\&nbsp;\s*/\&\#187; /g" > $*.html 
	$(TIDY) -m  $*.html

# Iso plain text target. The text file is generated from the HTML file.

%.iso.txt: %.html
	$(HTML2TXT) $*.html > $*.iso.txt

# Plain text target, with UTF-8 encoding, generated from the ISO-8859-15 text file.

%.txt: %.iso.txt
	$(ISO2UTF) $*.iso.txt > $*.txt

zip: $(SOURCES)
	tar zfcv tipe.tar.gz $(SOURCES)

clean:
	rm -f $(IMAGES) $(SCHEMAS) description.toc description.log description.aux figs.aux figs.log

clobber: clean
	rm -f tipe description.pdf figs.pdf
