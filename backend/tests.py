import unittest2
from algoritmusok import *

class TestAlgos(unittest2.TestCase):

    def test_sum(self):
        self.assertEqual(sum([1, 2, 3]), 6, "Should be 6")
    
    def test_toreturn(self):
        x = to_return(44, [0, 0, 1, 1, 0, 1, 3, 4, 1])
        self.assertEqual(x.maxprob, 44, "to_return: Wrong maxprob!")
        self.assertEqual(x.bestpar[5], 1, "to_return: Wrong bestpar element!")
        self.assertEqual(x.bestpar[7], 4, "to_return: Wrong bestpar element!")
    
    def test_countprob(self):
        self.assertEqual(count_prob([15, 3, 7, 2], [0, 0, 2]).maxprob, 1260, "count_prob: Wrong probability!")
        self.assertEqual(count_prob([23, 5, 12, 3, 6], [0, 0, 1, 2]).maxprob, 24840, "count_prob: Wrong probability!")
        self.assertEqual(count_prob([14, 4, 2, 6, 4, 1], [0, 0, 0, 3, 3]).maxprob, 13440, "count_prob: Wrong probability!")

    def test_simple_readin(self):
        lines = simple_readin("./testcases/simple.sims")
        self.assertEqual(len(lines), 100, "simple_readin: Wrong number of lines!")
        self.assertEqual(lines[5][6], 0.17635, "simple_readin: Wrong element!")
        self.assertEqual(lines[2], [1.0, 0.42444, 0.23864, 0.10732, 0.08530, 0.24087, 0.17860, 0.09548, 0.17341, 0.10233], "simple_readin: Wrong row!")
        self.assertIs(type(lines[17][3]), float, "simple_readin: Wrong conversion")
        self.assertIs(type(lines[55][6]), float, "simple_readin: Wrong conversion")
        self.assertIs(type(lines[81]), list, "simple_readin: Wrong type!")
        self.assertIs(type(lines[66]), list, "simple_readin: Wrong type!")
    
    def test_general_readin(self):
        matrices = general_readin("./testcases/general_10o.sims")
        self.assertEqual(len(matrices), 100, "general_readin: Wrong number of matrices!")
        self.assertEqual(matrices[3][7][2], 0.29851, "general_readin: Wrong element!")
        self.assertEqual(matrices[6][1][1], 0.39388, "general_readin: Wrong element!")
        self.assertEqual(matrices[88][8][0], 1.0, "general_readin: Wrong founder!")
        self.assertIs(type(matrices[88][5][5]), float, "general_readin: Wrong type!")

    def test_checkallposs(self):
        self.assertEqual(check_all_poss([9, 4, 2, 3, 1, 4, 3]).maxprob, 6480, "check_all_poss: Wrong probability")
        self.assertEqual(check_all_poss([8, 3, 4, 1, 2, 3]).bestpar, [0, 0, 2, 1, 2], "check_all_poss: Wrong parents")

    def test_general_count_prob(self):
        matrices = general_readin("./testcases/square.sims")
        self.assertAlmostEqual(general_count_prob_official(matrices[9], [0, 0, 2, 2, 3, 3, 1, 5, 5]).maxprob, 0.00001811695490826065021169191657, None,"general_count_prob: Wrong probability!", 0.0000001)
        self.assertAlmostEqual(general_count_prob_official(matrices[13], [0, 0, 2, 1, 1, 2, 6, 2, 4]).maxprob, 0.0000005115535090059749983781954, None,"general_count_prob: Wrong probability!", 0.0000001)

    def test_line_pre_check(self):
        data = simple_readin("./testcases/simple.sims")
        trypar = [-1]*(len(data[19])-1)
        line_pre_check(data[19], trypar)
        self.assertEqual(trypar, [0, 0, 2, -1, 3, -1, -1, -1, -1], "line_pre_check: Wrong list!")
    
    def test_simple_greedy(self):
        self.assertEqual(simple_greedy([11, 5, 4, 1, 1, 1]).bestpar, [0, 0, 1, 1, 2], "simple_greedy: Wrong list!")
        self.assertEqual(simple_greedy([14, 6, 5, 7]).maxprob, -1, "simple_greedy: Wrong maxprob!")

    def test_guess_solvable(self):
        self.assertIs(type(guess_solvable([15, 4, 5, 5, 3, 4, 2])), bool, "guess_solvable: Wrong type!")
        self.assertEqual(guess_solvable([9, 4, 3, 2, 6, 3, 4, 2, 1, 2]), False, "guess_solvable: Wrong outcome!")

    def test_full_pre_check(self):
        data = general_readin("./testcases/general_10o.sims")
        data1 = data[18]
        trypar = [-1]*(len(data1[0])-1)
        parentoptions = full_pre_check(data1, trypar)
        solution = [0, 1, 2, 3, 2, 2, 0, 7, 3]
        for i in range(len(solution)):
            self.assertTrue(parentoptions[i] in [solution[i], -1], "full_pre_check: Wrong outcome!")
        data = general_readin("./testcases/square.sims")
        data2 = data[7]
        trypar = [-1]*(len(data2[0])-1)
        parentoptions = full_pre_check(data2, trypar)
        solution = [0, 1, 0, 2, 3, 4, 6, 7, 3]
        for i in range(len(solution)):
            self.assertTrue(parentoptions[i] in [solution[i], -1], "full_pre_check: Wrong outcome!")

if __name__ == '__main__':
    unittest2.main()

