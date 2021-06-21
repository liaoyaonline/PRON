#!/bin/bash
#!/bin/bash
for a in {0..9}
do
    dot -Tpdf $a -o $a.pdf
done
