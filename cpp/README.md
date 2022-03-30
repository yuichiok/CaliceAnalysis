# CaliceAnalysis
Personal monologue to keep track of findings

#### Variables
*1 variable per event*
- `event`\
	This is event id / cycle
- `spill`\
	Legacy branch
- `cycle`\
	Number of cycles. Increments over time. Typically 1ms + 10ms delay between cycles but depends on run.
- `bcid`\
	Also increments over time but resets every 4096. `bcid % 4096` is ok.
- `bcid_first_sca_full`\
	Identify the bcid that fills up first sca.
- `bcid_merge_end`\
	What this?
- `bcid_prev`\
	What this?
- `bcid_next`\
	What this?
- `id_run`\
	Gives the ID of the runs. e.g. 050282?
- `id_dat`\
	Gives ID of dat file that is working on. When merging, this is mixed up. *It does not merge files in order of dat_id!* When dealing with time sensitive variables, make sure these are sorted. (e.g. plotting the hit frequency)
- `nhit_slab`\
	Number of slabs in coincidence. Currently put >= 6 as a preselection cut.
- `nhit_chip`\
	Number of chips that's been hit.
- `nhit_chan`\
	Number of channels that's been hit.
- `nhit_len`\
	The total number of hits in a event. (all slabs, chips, channels.)
- `sum_hg`\
	Summed high gain.
- `sum_energy`\
	Summed energy.
- `sum_energy_lg`\
	Summed low gain energy.

*Per event per hit*
- `hit_slab[1949]`\
	n th layer of this hit
- `hit_chip[1949]`\
	n th chip of this hit
- `hit_chan[1949]`\
	n th channel of this hit
- `hit_sca[1949]`\
	n th sca of this hit
- `hit_x[1949]`\
	x coordinate of this hit
- `hit_y[1949]`\
	y coordinate of this hit
- `hit_z[1949]`\
	z coordinate of this hit --> Each layer has the same z coordinate.
- `hit_hg[1949]`\
	High gain of this hit
- `hit_lg[1949]`\
	Low gain of this hit
- `hit_energy[1949]`\
	Energy of this hit
- `hit_energy_lg[1949]`\
	Low gain energy of this hit
- `hit_n_scas_filled[1949]`\
	Number of scas fill from this hit?
- `hit_isHit[1949]`\
	Is hit?
- `hit_isMasked[1949]`\
	Is masked?
- `hit_isCommissioned[1949]`\
	Is commissioned???

