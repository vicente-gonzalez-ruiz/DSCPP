#
# Makefile para SDLC++
# gse. 2000.
#
#
# Opciones de compilaci'on.
#
CC=g++
CFLAGS= -g

#
# Comando de borrado de ficheros del sistema operativo
#
RM=rm -f

#
# objetivos
#
EXE = test_clock test_random test_monostable\
\
 test_not test_and test_and3\
 test_and3b test_or\
 test_or3 test_nand test_nand3 test_nand3b test_nor test_nor3\
 test_xor test_xor3 test_nxor test_nxor3 test_buffer_z\
\
 test_decod_2_4 test_decod_5_32\
 test_decod_2_4_sel test_decod_5_32_sel test_decod\
\
 test_mux_2_1 test_demux_1_2 test_mux_4_1 test_demux_1_4\
 test_mux test_demux\
\
 test_sr_latch_nor test_sr_latch_nand test_d_latch\
 test_jk_latch test_jk_latch_enable\
 test_sr_latch_enable\
\
 test_positive_detector test_negative_detector\
 test_negative_glich_supressor test_positive_glich_supressor\
 test_neg_sr_ff test_neg_d_ff test_neg_jk_ff test_neg_jk_ff_pc\
 test_neg_d_ff_pc test_sr_ff_ms test_jk_ff_ms test_paco\
\
 test_reg_8\
\
 test_au_4bin_counter test_au_8bin_counter\
 test_ad_4bin_counter test_au_bcd_counter test_au_bcd_counter_clr\
 test_su_4bin_counter\
 test_su_8bin_counter test_su_bin_counter\
\
 test_sram_bit test_sram_16x4 test_sram_16x4_b\
\
 test_fr_bit test_fr_32x32\
\
 test_adder_bit test_adder_bit_cla test_alu_bit test_alu_bit_cla\
 test_alu_msbit test_alu_32bit_rc test_alu_32bit_cla\
\
 asm2r2k
# test_alu_control test_r2000_control

# Generadores de bits
test_clock:	test_clock.c clock.o
		$(CC) $(CFLAGS) $< clock.o -o $@
test_random:	test_random.c random.o
		$(CC) $(CFLAGS) $< random.o -o $@
test_monostable:test_monostable.c monostable.o
		$(CC) $(CFLAGS) $< monostable.o -o $@

# Puertas l'ogicas
test_not:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_NOT_ $< gates.o clock.o -o $@
test_and:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_AND_ $< gates.o clock.o -o $@
test_and3:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_AND3_ $< gates.o clock.o -o $@
test_and3b:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_AND3b_ $< gates.o clock.o -o $@
test_or:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_OR_ $< gates.o clock.o -o $@
test_or3:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_OR3_ $< gates.o clock.o -o $@
test_nand:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_NAND_ $< gates.o clock.o -o $@
test_nand3:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_NAND3_ $< gates.o clock.o -o $@
test_nand3b:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_NAND3b_ $< gates.o clock.o -o $@
test_nor:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_NOR_ $< gates.o clock.o -o $@
test_nor3:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_NOR3_ $< gates.o clock.o -o $@
test_xor:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_XOR_ $< gates.o clock.o -o $@
test_xor3:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_XOR3_ $< gates.o clock.o -o $@
test_nxor:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_NXOR_ $< gates.o clock.o -o $@
test_nxor3:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_NXOR3_ $< gates.o clock.o -o $@
test_buffer_z:	test_gates.c gates.o clock.o
		$(CC) $(CFLAGS) -D_BUFFER_Z_ $< gates.o clock.o -o $@

# Descodificadores
test_decod_2_4:		test_decods.c decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_DECOD_2_4_ $< decods.o gates.o clock.o -o $@
test_decod_5_32:	test_decods.c decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_DECOD_5_32_ $< decods.o gates.o clock.o -o $@
test_decod_2_4_sel:	test_decods.c decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_DECOD_2_4_SEL_ $< decods.o gates.o clock.o -o $@
test_decod_5_32_sel:	test_decods.c decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_DECOD_5_32_SEL_ $< decods.o gates.o clock.o -o $@
test_decod:		test_decods.c decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_DECOD_ $< decods.o gates.o clock.o -o $@

# Multiplexores y desmultiplexores
test_mux_2_1:	test_muxs.c muxs.o decods.o gates.o clock.o
		$(CC) $(CFLAGS) -D_MUX_2_1_ $< muxs.o decods.o gates.o clock.o -o $@
test_mux_4_1:	test_muxs.c muxs.o decods.o gates.o clock.o
		$(CC) $(CFLAGS) -D_MUX_4_1_ $< muxs.o decods.o gates.o clock.o -o $@
test_mux:	test_muxs.c muxs.o decods.o gates.o clock.o
		$(CC) $(CFLAGS) -D_MUX_ $<  muxs.o decods.o gates.o clock.o -o $@
test_demux_1_2:	test_muxs.c muxs.o decods.o gates.o clock.o
		$(CC) $(CFLAGS) -D_DEMUX_1_2_ $< muxs.o decods.o gates.o clock.o -o $@
test_demux_1_4:	test_muxs.c muxs.o decods.o gates.o clock.o
		$(CC) $(CFLAGS) -D_DEMUX_1_4_ $< muxs.o decods.o gates.o clock.o -o $@
test_demux:	test_muxs.c muxs.o decods.o gates.o clock.o
		$(CC) $(CFLAGS) -D_DEMUX_ $< muxs.o decods.o gates.o clock.o -o $@

# Cerrojos
test_sr_latch_nor:	test_latches.c latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_SR_LATCH_NOR_ $< latches.o gates.o clock.o -o $@
test_sr_latch_nand:	test_latches.c latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_SR_LATCH_NAND_ $< latches.o gates.o clock.o -o $@
test_d_latch:		test_latches.c latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_D_LATCH_ $< latches.o gates.o clock.o -o $@
test_jk_latch:		test_latches.c latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_JK_LATCH_ $< latches.o gates.o clock.o -o $@
test_jk_latch_enable:	test_latches.c latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_JK_LATCH_ENABLE_ $< latches.o gates.o clock.o -o $@
test_sr_latch_enable:	test_latches.c latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_SR_LATCH_ENABLE_ $< latches.o gates.o clock.o -o $@

# flip-flops
test_positive_detector:	test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_POSITIVE_DETECTOR_ $< flip-flops.o latches.o gates.o clock.o -o $@
test_negative_detector:	test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_NEGATIVE_DETECTOR_ $< flip-flops.o latches.o gates.o clock.o -o $@
test_negative_glich_supressor:	test_flip-flops.c flip-flops.o latches.o gates.o clock.o random.o
			$(CC) $(CFLAGS) -D_NEGATIVE_GLICH_SUPRESSOR_ $< flip-flops.o latches.o gates.o clock.o random.o -o $@
test_positive_glich_supressor:	test_flip-flops.c flip-flops.o latches.o gates.o clock.o random.o
			$(CC) $(CFLAGS) -D_POSITIVE_GLICH_SUPRESSOR_ $< flip-flops.o latches.o gates.o clock.o random.o -o $@
test_neg_sr_ff:		test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_NEG_SR_FF_ $< flip-flops.o latches.o gates.o clock.o -o $@
test_neg_d_ff:		test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_NEG_D_FF_ $< flip-flops.o latches.o gates.o clock.o -o $@
test_neg_jk_ff:		test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_NEG_JK_FF_ $< flip-flops.o latches.o gates.o clock.o -o $@
test_neg_jk_ff_pc:	test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_NEG_JK_FF_PC_ $< flip-flops.o latches.o gates.o clock.o -o $@
test_neg_d_ff_pc:	test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_NEG_D_FF_PC_ $< flip-flops.o latches.o gates.o clock.o -o $@
test_paco:		test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_PACO_ $< flip-flops.o latches.o gates.o clock.o -o $@
test_sr_ff_ms:		test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_SR_FF_MS_ $< flip-flops.o latches.o gates.o clock.o -o $@
test_jk_ff_ms:		test_flip-flops.c flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_JK_FF_MS_ $< flip-flops.o latches.o gates.o clock.o -o $@

# Registros
test_reg_8:		test_regs.c regs.o flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_REG_8_ $< regs.o flip-flops.o latches.o gates.o clock.o -o $@

#Contadores
test_au_4bin_counter:	test_counters.c counters.o flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_AU_4BIN_COUNTER_ $< counters.o flip-flops.o latches.o gates.o clock.o -o $@
test_au_8bin_counter:	test_counters.c counters.o flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_AU_8BIN_COUNTER_ $< counters.o flip-flops.o latches.o gates.o clock.o -o $@
test_ad_4bin_counter:	test_counters.c counters.o flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_AD_4BIN_COUNTER_ $< counters.o flip-flops.o latches.o gates.o clock.o -o $@
test_au_bcd_counter:	test_counters.c counters.o flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_AU_BCD_COUNTER_ $< counters.o flip-flops.o latches.o gates.o clock.o -o $@
test_au_bcd_counter_clr:test_counters.c counters.o flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_AU_BCD_COUNTER_CLR_ $< counters.o flip-flops.o latches.o gates.o clock.o -o $@
test_su_4bin_counter:	test_counters.c counters.o flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_SU_4BIN_COUNTER_ $< counters.o flip-flops.o latches.o gates.o clock.o -o $@
test_su_8bin_counter:	test_counters.c counters.o flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_SU_8BIN_COUNTER_ $< counters.o flip-flops.o latches.o gates.o clock.o -o $@
test_su_bin_counter:	test_counters.c counters.o flip-flops.o latches.o gates.o clock.o
			$(CC) $(CFLAGS) -D_SU_BIN_COUNTER_ $< counters.o flip-flops.o latches.o gates.o clock.o -o $@

# SRAMS
test_sram_bit:		test_srams.c srams.o latches.o muxs.o decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_SRAM_BIT_ $< srams.o latches.o muxs.o decods.o gates.o clock.o -o $@
test_sram_16x4:		test_srams.c srams.o latches.o muxs.o decods.o gates.o clock.o random.o
			$(CC) $(CFLAGS) -D_SRAM_16x4_ $< srams.o latches.o muxs.o decods.o gates.o clock.o random.o -o $@
test_sram_16x4_b:	test_srams.c srams.o latches.o muxs.o decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_SRAM_16x4_B_ $< srams.o latches.o muxs.o decods.o gates.o clock.o -o $@

# Ficheros de registros
test_fr_bit:		test_fr32x32.c fr32x32.o flip-flops.o latches.o decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_FR_BIT_ $< fr32x32.o flip-flops.o latches.o decods.o gates.o clock.o -o $@
test_fr_32x32:		test_fr32x32.c fr32x32.o flip-flops.o latches.o decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_FR_32x32_ $< fr32x32.o flip-flops.o latches.o decods.o gates.o clock.o -o $@

# Unidades aritm'etico-l'ogicas
test_adder_bit:		test_alu32.c alu32.o muxs.o decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_ADDER_BIT_ $< alu32.o muxs.o decods.o gates.o clock.o -o $@
test_adder_bit_cla:	test_alu32.c alu32.o muxs.o decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_ADDER_BIT_CLA_ $< alu32.o muxs.o decods.o gates.o clock.o -o $@
test_alu_bit:		test_alu32.c alu32.o muxs.o decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_ALU_BIT_ $< alu32.o muxs.o decods.o gates.o clock.o -o $@
test_alu_bit_cla:	test_alu32.c alu32.o muxs.o decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_ALU_BIT_CLA_ $< alu32.o muxs.o decods.o gates.o clock.o -o $@
test_alu_msbit:		test_alu32.c alu32.o muxs.o decods.o gates.o clock.o
			$(CC) $(CFLAGS) -D_ALU_MSBIT_ $< alu32.o muxs.o decods.o gates.o clock.o -o $@
test_alu_32bit_rc:	test_alu32.c alu32.o muxs.o decods.o gates.o clock.o random.o
			$(CC) $(CFLAGS) -D_ALU_32BIT_RC_ $< alu32.o muxs.o decods.o gates.o clock.o random.o -o $@
test_alu_32bit_cla:	test_alu32.c alu32.o muxs.o decods.o gates.o clock.o random.o
			$(CC) $(CFLAGS) -D_ALU_32BIT_CLA_ $< alu32.o muxs.o decods.o gates.o clock.o random.o -o $@

# Ensamblador de MIPS R2000
asm2r2k:		asm2r2k.c
			$(CC) $(CFLAGS) $< -o $@

#Unidades de control
test_alu_control:	ucs.c ucs.h flip-flops.o latches.o decods.o muxs.o gates.o clock.o random.o
			$(CC) $(CFLAGS) -D_ALU_CONTROL_ $< flip-flops.o latches.o decods.o muxs.o gates.o clock.o random.o -o $@
test_r2000_control:	ucs.c ucs.h flip-flops.o latches.o decods.o muxs.o gates.o clock.o random.o
			$(CC) $(CFLAGS) -D_R2000_CONTROL_ $< flip-flops.o latches.o decods.o muxs.o gates.o clock.o random.o -o $@

#
# Crea un fichero de biblioteca 'unico
#
SOURCES	=	clock.c random.c monostable.c gates.c decods.c \
		muxs.c latches.c flip-flops.c regs.c srams.c \
		counters.c fr32x32.c

OBJECTS	=	${SOURCES:.c=.o}

RANLIB	=	ranlib

sdlc++.a:	${OBJECTS}
		ar rv sdlc++.a ${OBJECTS}
		${RANLIB} sdlc++.a

#
# Lo crea todo
#
all:	$(EXE) #sdlc++.a

#
# Compila todas las bibliotecas (.o)
#
.c.o:
	$(CC) -c $(CFLAGS) $<

#
# Borra todos los ficheros que no sean fuentes
#
clean:
	$(RM) *.o *.bak *~ $(EXE) sdlc++.a data data.sdl *.r2k

#
# Muestra los posibles objetivos
#
show:
	@echo "Posibles objetivos: "
	@echo $(EXE)

#
# Muestra informaci'on
#
info:
	@echo "Este es el fichero de depencias de SDLC++"
	make show
	@echo "gse. 2000"
