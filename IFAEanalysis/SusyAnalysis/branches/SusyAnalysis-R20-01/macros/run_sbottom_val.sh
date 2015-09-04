
file1=/nfs/at3/scratch/tripiana/StopAnalysis/anafiles/DC14//Nom_TestTruthMC15.root
file2=/nfs/at3/scratch/tripiana/StopAnalysis/anafiles/DC14//Nom_TestTruthDC14.root
label1="MC15"
label2="DC14"

root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_N\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_N40\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_N60\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_N80\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"bj_N\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"e_N\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"m_N\")"

root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"e_pt[0]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"e_eta[0]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"e_phi[0]\")"

root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"m_pt[0]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"m_eta[0]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"m_phi[0]\")"

root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_pt[0]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_pt[1]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_pt[2]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_pt[3]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_pt[4]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_pt[5]\")"

root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_eta[0]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_eta[1]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_eta[2]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_eta[3]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_eta[4]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_eta[5]\")"

root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_phi[0]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_phi[1]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_phi[2]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_phi[3]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_phi[4]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"j_phi[5]\")"

root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"met[0]\")"
root -b -q "comparevars.cxx(\"$file1\",\"$file2\",\"$label1\",\"$label2\",\"MC15val\",\"met[1]\")"