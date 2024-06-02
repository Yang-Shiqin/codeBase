"""
尝试解耦的，调用sumo的pso自校准，改自module_xxx
"""

import datetime
import time
import matplotlib.pyplot as plt
import multiprocessing as mp
import numpy as np
import pandas as pd
import pyswarms as ps  # 引入粒子群优化模块
from loguru import logger

from pso_base import PSOBase


class PSO_SelfCalibration(PSOBase):
    def __init__(self, config='D:/ysq/DT_Self_Calibration/src/config/pso_template.yaml'):
        super(PSO_SelfCalibration, self).__init__(config)
        self.pos_name = [[  # 待优化的参数
                'Sumo__Plant__param__Sumo2__YOHO_SB_anox',      # 阶段1
                'Sumo__Plant__param__Sumo2__YOHO_SB_ox',        # 阶段1
                'Sumo__Plant__param__Sumo2__muAOB',    # 硝化菌最大比生长率, 阶段1
                'Sumo__Plant__param__Sumo2__bAOB',              # 阶段2
                'Sumo__Plant__param__Sumo2__KNHx_AOB_AS',       # 阶段2
                'Sumo__Plant__param__Sumo2__KO2_AOB_AS',        # 阶段2
                'Sumo__Plant__PAC1__param__G',                  # 阶段2
                'Sumo__Plant__PAC__param__G',                   # 阶段2
                'Sumo__Plant__Clarifier2__param__fXTSS_sludge_base',    # 阶段2
                'Sumo__Plant__CSTR1_4__param__alpha',           # 阶段1
            ],[
                'Sumo__Plant__CSTR2_4__param__alpha',           # 阶段1
            ],[
                'Sumo__Plant__CSTR3_4__param__alpha',           # 阶段1
            ],[
                'Sumo__Plant__CSTR4_4__param__alpha',           # 阶段1
            ]]


    def _init_parameters(self):
        super(PSO_SelfCalibration, self)._init_parameters()
        self.n_threads = int(self.cfg["n_threads"])

    # 重写目标函数
    def get_loss(self, weights, result) -> list:
        loss1 = weights['tn']*(np.nansum(np.abs(result['Sumo__Plant__Effluent__TN'].to_numpy() \
            -self.effluent_data['effluent_dis1_tn'].to_numpy()))/self.std_eff['effluent_dis1_tn']) \
            + weights['do']*(np.nansum(np.abs(result['Sumo__Plant__CSTR1_8__SO2'].to_numpy() \
            -self.operating_data['aao1_1_aerobic_terminal_do'].to_numpy()))/self.std_ope['aao1_1_aerobic_terminal_do']/4) \
            + weights['do']*(np.nansum(np.abs(result['Sumo__Plant__CSTR2_8__SO2'].to_numpy() \
            -self.operating_data['aao1_2_aerobic_terminal_do'].to_numpy()))/self.std_ope['aao1_2_aerobic_terminal_do']/4) \
            + weights['do']*(np.nansum(np.abs(result['Sumo__Plant__CSTR3_8__SO2'].to_numpy() \
            -self.operating_data['aao2_1_aerobic_terminal_do'].to_numpy()))/self.std_ope['aao2_1_aerobic_terminal_do']/2)
        
        loss2 = weights['snhx']*(np.nansum(np.abs(result['Sumo__Plant__Effluent__SNHx'].to_numpy() \
            -self.effluent_data['effluent_secpump_snhx'].to_numpy()))/self.std_eff['effluent_secpump_snhx']) \
            + weights['tp']*(np.nansum(np.abs(result['Sumo__Plant__Effluent__TP'].to_numpy() \
            -self.effluent_data['effluent_dis1_tp'].to_numpy()))/self.std_eff['effluent_dis1_tp']) \
            + weights['ss']*(np.nansum(np.abs(result['Sumo__Plant__Effluent__XTSS'].to_numpy() \
            -self.effluent_data['effluent_dis1_ss'].to_numpy()))/self.std_eff['effluent_dis1_ss'])
        return [loss1, loss2]

    # 解包,便于投到进程池中进行处理
    @staticmethod
    def wrapper(args):
        func = args[0]
        func_args = args[1:]
        return func(*func_args)

    def calc_loss1(self, x):
        '''根据出水约束和目标优化计算损失函数'''
        # 采用多进程池子同时处理每个粒子的寻优 total_res是包含所有粒子的出水结果
        # [ ] TODO: 这里写的很不好，最好写到配置文件里
        if "Sumo__Plant__param__Sumo2__muAOB" in self.pos_name[0]:
            n_pos = 3
        else:
            n_pos = 2
        tasks = []
        for i, r in enumerate(x):
            # 构造pos
            pos = self.best_pos.copy()
            for j in range(n_pos):
                pos[self.pos_name[0][j]] = r[j]
            if self.alpha_mode == True:
                for j in range(4):
                    pos[self.pos_name[j][-1]] = r[-1]

            tasks.append((self.run_one_pos, self.dur_day, i, pos))
        pool = mp.Pool()
        res = pool.map(self.wrapper, tasks)
        fit_arr = [i[1][0] for i in res]
        tbest = min(fit_arr)
        gbest = min([tbest]+self.draw_gbest1)
        self.draw_gbest1.append(gbest)
        self.draw_tbest1.append(tbest)

        logger.debug(f"x={x}")
        logger.debug(f"loss={fit_arr}")
        return fit_arr

    def calc_loss2(self, x, pos):
        '''根据出水约束和目标优化计算损失函数'''
        tasks = []
        if "Sumo__Plant__param__Sumo2__muAOB" in self.pos_name[0]:
            n_pos = 3
        else:
            n_pos = 2
        for i, r in enumerate(x):
            # 构造pos
            pos2 = self.best_pos.copy()
            for j in range(n_pos):
                pos2[self.pos_name[0][j]] = pos[j]
            for j in range(6):
                pos2[self.pos_name[0][n_pos+j]] = r[j]
            if self.alpha_mode == True:
                for j in range(4):
                    pos2[self.pos_name[j][-1]] = pos[-1]
            tasks.append((self.run_one_pos, self.dur_day, i, pos2))

        pool = mp.Pool()
        res = pool.map(self.wrapper, tasks)
        fit_arr = [i[1][1] for i in res]

        tbest = min(fit_arr)
        gbest = min([tbest]+self.draw_gbest2)
        self.draw_gbest2.append(gbest)
        self.draw_tbest2.append(tbest)

        logger.debug(f"x={x}")
        logger.debug(f"loss={fit_arr}")

        return fit_arr

    # 增加muB
    def pso_Optimize1(self, iters):
        '''粒子群优化函数'''

        # 设置hyperparameters
        options = {'c1': 1.2, 'c2': 1.4, 'w': 0.4}
        n_particles = self.n_threads
        if "Sumo__Plant__param__Sumo2__muAOB" in self.pos_name[0]:
            n_pos = 3
            min_bound = np.array([0.4, 0.5, 0.4])
            max_bound = np.array([0.7, 0.8, 1.1])
        else:
            n_pos = 2
            min_bound = np.array([0.4, 0.5])
            max_bound = np.array([0.7, 0.8])

        # 设置范围
        i1 = [*[self.best_pos[i] for i in self.pos_name[0][:n_pos]]]    # 设置参数寻优初始值
        if self.alpha_mode == True:    # 自校正alpha
            min_bound = np.append(min_bound, -0.0711*0.3*5)
            max_bound = np.append(max_bound, -0.0711*0.0001)
            i1 += [-0.0711*0.0001]
            # i1 = [0.69, 0.62, -0.0711*0.0001]
        bounds = (min_bound, max_bound)
        print(i1)
        dimensions = len(min_bound)
        min_bounds = np.repeat(np.array(min_bound)[np.newaxis, :], n_particles, axis=0)
        max_bounds = np.repeat(np.array(max_bound)[np.newaxis, :], n_particles, axis=0)
        init_pos = np.random.uniform(low=min_bounds, high=max_bounds, size=(n_particles, dimensions))
        init_pos[0] = i1
        # 调用PSO实例
        optimizer = ps.single.GlobalBestPSO(
            n_particles=n_particles, dimensions=dimensions, options=options, bounds=bounds, init_pos=init_pos)

        self.draw_gbest1 = []
        self.draw_tbest1 = []
        loss, pos = optimizer.optimize(self.calc_loss1, iters=int(iters))
        self.draw_gbest1 = pd.DataFrame(self.draw_gbest1)
        self.draw_tbest1 = pd.DataFrame(self.draw_tbest1)
        for c in self.draw_gbest1:
            plt.plot(self.draw_gbest1[c])
            plt.savefig(f'{self.out_dir}/gbest1_{c}.png')
            plt.clf()
        for c in self.draw_tbest1:
            plt.plot(self.draw_tbest1[c])
            plt.savefig(f'{self.out_dir}/tbest1_{c}.png')
            plt.clf()

        # 返回最优曝气值
        logger.debug(f'cost:{loss}, pos:{pos}')
        return pos, loss

    def pso_Optimize2(self, iters, pos):
        '''粒子群优化函数'''

        # 设置hyperparameters
        options = {'c1': 1.2, 'c2': 1.4, 'w': 0.4}

        # 设置范围
        min_bound = np.array([0.11, 0.42, 0.15, 20, 20, 0.82])
        max_bound = np.array([0.23, 0.98, 0.35, 140, 140, 0.99])
        bounds = (min_bound, max_bound)

        n_particles = self.n_threads
        dimensions = len(min_bound)
        min_bounds = np.repeat(np.array(min_bound)[np.newaxis, :], n_particles, axis=0)
        max_bounds = np.repeat(np.array(max_bound)[np.newaxis, :], n_particles, axis=0)
        init_pos = np.random.uniform(low=min_bounds, high=max_bounds, size=(n_particles, dimensions))
        init_pos[0] = [self.best_pos[i] for i in self.pos_name[0][2:2+6]]
        # init_pos[0] = [0.12, 0.45, 0.2, 100, 120, 0.92]
        print(init_pos[0])
        # 调用PSO实例
        optimizer = ps.single.GlobalBestPSO(
            n_particles=n_particles, dimensions=dimensions, options=options, bounds=bounds, init_pos=init_pos)

        self.draw_gbest2 = []
        self.draw_tbest2 = []
        loss, pos = optimizer.optimize(self.calc_loss2, iters=int(iters), pos=pos)
        self.draw_gbest2 = pd.DataFrame(self.draw_gbest2)
        self.draw_tbest2 = pd.DataFrame(self.draw_tbest2)
        for c in self.draw_gbest2:
            plt.plot(self.draw_gbest2[c])
            plt.savefig(f'{self.out_dir}/gbest2_{c}.png')
            plt.clf()
        for c in self.draw_tbest2:
            plt.plot(self.draw_tbest2[c])
            plt.savefig(f'{self.out_dir}/tbest2_{c}.png')
            plt.clf()
        logger.info(f'best_cost = {loss}')

        # 返回最优曝气值
        return pos, loss

    def self_calibration(self, iters):
        # 阶段1的PSO优化参数
        pos1, loss1 = self.pso_Optimize1(iters[0])
        
        # 阶段2的PSO优化参数
        pos2, loss2 = self.pso_Optimize2(iters[1], pos1)
        print(f'pos1:{pos1}, pos2:{pos2}')

        logger.info(f"yoho_sb_anox = {pos1[0]}\n"                       # 0.69
                    f"yoho_sb_ox = {pos1[1]}\n"                         # 0.62
                    f"aob_muB = {pos1[2] if 'Sumo__Plant__param__Sumo2__muAOB' in self.pos_name[0] else None}\n"  # 0.62
                    f"alpha = {pos1[-1]}\n"                             # 1
                    f"baob = {pos2[0]}\n"                               # 0.12
                    f"knhx_aob_as = {pos2[1]}\n"                        # 0.45
                    f"ko2_aob_as = {pos2[2]}\n"                         # 0.2
                    f"pac1 = {pos2[3]}\n"                               # 100
                    f"pac2 = {pos2[4]}\n"                               # 120
                    f"pac3 = {pos2[3]}\n"
                    f"clarifier2_fxtss_sludge_base = {pos2[5]}\n"       # 0.92
                    f"clarifier2_fxtss_sludge_polymer = {pos2[5]}\n"
                    )
        pos = self.best_pos.copy()
        print(pos)
        if "Sumo__Plant__param__Sumo2__muAOB" in self.pos_name[0]:
            n_pos = 3
        else:
            n_pos = 2
        for j in range(n_pos):          # YOHO_SB_anox, YOHO_SB_ox, muB
            pos[self.pos_name[0][j]] = pos1[j]
        for j in range(6):              # bAOB, KNHx_AOB_AS, KO2_AOB_AS, PAC1, PAC2, fXTSS_sludge_base
            pos[self.pos_name[0][n_pos+j]] = pos2[j]
        if self.alpha_mode == True:     # alpha系数
            for j in range(4):
                pos[self.pos_name[j][-1]] = pos1[-1]
        return pos, loss1+loss2
    
def main():
    start = time.time()
    a = PSO_SelfCalibration()
    logger.add(a.out_dir / f'pso_info.log', filter=lambda record: record['level'].name =='INFO')
    logger.add(a.out_dir / f'pso_debug.log', filter=lambda record: record['level'].name =='DEBUG')
    best_pos, best_loss, ALARM = a.run(now=datetime.datetime(2023,4,11,1)+datetime.timedelta(days=1*7), dur_day=7, iters=[6,6], alpha_mode=True)
    logger.info('======================\n'
                f'best_pos: {best_pos}, best_loss: {best_loss}, ALARM: {ALARM}\n'
                '=====================================================================\n')
    end = time.time()
    logger.info(f"{end-start}")


if __name__ == "__main__":
    main()
