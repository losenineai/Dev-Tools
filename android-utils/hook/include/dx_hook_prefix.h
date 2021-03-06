#ifndef dx_hook_prefix_h
#define dx_hook_prefix_h

// x86
#define MSLogHexEx						                        iOgoLlO0l11iiLg9o1g00L19ool0lL0O
#define MSLogHex                                                OgO1iIloo0Li9oOogI1I1o1Ogi0giOio
                        
#define MSGetInstructionWidth			                        g0l0I9o91LoIl1iOolIiLgOOg00ioi0L
#define MSHookFunction                                          o9ool10l9ologI0gO00IOi0Lg0OOILgL
#define SubstrateHookFunctionThumb                              IOli1oiLOg1g0O9I99gLo1LI1Lo990Og
                        
#define SubstrateHookMemory                                     O0io9i0g9LooogLLOI01I9oog0Ogooii
#define SubstrateMemoryCreate                                   o11gOgo1gOlliI9goliooliiig9LglgL
#define SubstrateMemoryRelease                                  g9OgO0olo0Ii1g9iL0lIlLo00iIO9oOI
                        
#define hde64_disasm                                            og9gigLoII0gilllo0oil0g0L1010lio
                        
#define utils							                        I1LLo99L1Oo00L0ioog0gliL0O1oLlii
                        
#define dx_hook_hookfun					                        IiO1llgLolI99IlO0oLilioiO9gIo1II
#define dx_hook_debug                                           IOI1l0IOgoI0II11i0ooLLo190g1llIL
#define dx_hook_make_as_system_caller                           IOI1l0IOiiiiiiiiiiooLLo190g1llIL

// x64
#define DebugLogControlerEnableLog                              l0iI10il0Io1LoIggoOO1IL9lil1l190
#define DebugLogControlerIsEnableLog                            iiLigiiIgog1O1l0iLi0oOO9glL0gLgi
#define DebugLogControlerIsEnableDebugbreak                     IgL0oI0Ogili0109olil0ILgi9g9io1i
#define DebugLogControlerSharedInstance                         i1LIgO90gooiOi0IloggL1LLgiI0ilOi
#define Log_TrampolineBuildForEnterTransfer                     llL00O19giooI9OL0O0ggLil0o1iIOI1
#define Log_TrampolineBuildForEnter                             O0lgll09lOOLoioiI099iO11i19i19Ll
#define Log_TrampolineBuildForInvoke                            O9o9Ogi0L9goIIll990lgOLOOl1ggoIL
#define Log_TrampolineBuildForLeave                             L01oi1i119lOg9gogoo9LL1Ogo0og19o
#define InterceptorInitialize                                   OOlo1l10l0Il99g11gO9Ill0L9i99LO0
#define InterceptorFindHookEntry                                OiIlOO1L0199ggLgg0glglOii900L1ol
#define InterceptorSharedInstance                               OL0OOoLgOlIi11llO09lI1ooLlggL0II
#define InterceptorAddHookEntry                                 O0oO0i00Ol9lO1o90oOogi00o9gIllgI
#define HookEntryInitialize                                     ILgigoLllLloi0lgoL1iliIO90Il9o99
#define ZzFreeHookEntry                                         lO0009091091gIiLglL0iOLI01gI001l
#define ZzBuildHook                                             i11I0Iio909g1IoooloiLI0OOlol1o0O
#define ZzBuildHookGOT                                          OIgIoIOOgi9OO1g9l99i111g0Il00o9i
#define ZzEnableHook                                            g1g1LLl010I009oOli9ioI1g91gIg10I
#define ZzDisableHook                                           LggIO0L9gooOLg1i0o0g9I1OOLo90ggl
#define ZzHook                                                  OIi090LgIlL0OIl90L9o0il1g0LL1ii0
#define ZzHookPrePost                                           gg0I1ILIiggoOI0l0oOi91lioO1OIogi
#define ZzHookReplace                                           OoL1gIil1i9lO911Llo0gLoi99i1i099
#define ZzDynamicBinaryInstrumentation                          L1lLILiiOLl0L0oooIIII09I9LI1I0i0
#define malloc0                                                 IL1ig9o9i1lL9oL1gliOi1oi1g1g0goO
#define ZzRuntimeCodePatch                                      oLigl0LlIOOglL19i1g0I0Lgg9I9Ii91
#define ExecuteMemoryManagerSharedInstance                      o09019oLOiliLLll0lOLgooil0IoO10L
#define AllocateNearExecuteMemoryPage                           g0Lg0gLLiIO0g9O9oOoigLg9iIlgi09L
#define ExecuteMemoryManagerAdd                                 L99LLgOLg0lllo1Ilg009io0IO0lO0oi
#define ExecuteMemoryManagerAllocateCodeSlice                   gl909O0I10Iggio1lLLoiI1IiLLOogOI
#define ExecuteMemoryManagerAllocateNearCodeSlice               iiO10OiLo1i0l1gLlL9o0g90lIg9I90o
#define ThreadStackGetByThreadLocalKey                          gLgiIgoggo9oIigLgOI00OiIgiIo010L
#define ThreadStackAllocate                                     ol0IIll11i1Oli0gOiOiIi0gL9OIgo10
#define CallStackAllocate                                       o1i9oo00ILlgi0i90lIilLg0gLLooog9
#define CallStackFree                                           oL1iOI1l99Ii1l11llO1LgLi9OL19l09
#define ThreadStackPopCallStack                                 glLog9ooOiogiO0OI1Lil9900L1iLi9i
#define ThreadStackPushCallStack                                gOlL911LoLLO01L1iLg1LgOiLol0OL9i
#define CallStackGetThreadLocalData                             II9iIgIgo0II9oglIi0l0oOllLl9llII
#define CallStackAllocateData                                   IO1IOog9olOlIiO1OOill0IOoIooIOl1
#define CallStackSetThreadLocalData                             lo9O1l1LIglgi0LL0gIL1oOOiL91gglL
#define TrampolineBuildAll                                      IgOOoL1oIIIOg0lOIlg9oOO1i9L099O1
#define ZzHookGOT                                               loLLlI9iLo9LiO0ll90L0OIilL91IOIg
#define ZzDisableHookGOT                                        IIg9110Oi9lIOo0ig9IgIiI1I1i11ogi
#define MemoryHelperGetPageSize                                 g9Oo9I9lIL999olgI0LLoolI9l91gl11
#define MemoryHelperAllocatePage                                g0oOi11lOL1ioOoO110IIOlgLg9oIg9i
#define MemoryHelperAllocateNearPage                            IIL19O1ggLI9oOLllL901iLOIiLIlI9g
#define MemoryAllocate                                          iOLigLo1Ig9I1o1LLoI9L0gg9OI0LIII
#define MemoryHelperPatchCode                                   I0liig0iLoooiiigii0l19ol0iggLL10
#define MemoryHelperProtectAsExecutable                         gl0o10IoI9Oi0OLLl0Ig01OLolilIl9i
#define MemoryHelperProtectAsWritable                           IO0log0o9OlIig0oOIl10iligLiog9o0
#define MemoryHelperSearchCodeCave                              ig9iliLo9iLOloiIIi9oLLlLilI1oIg0
#define MemoryHelperIsSupportAllocateRXMemory                   lgiO91OoIlIiI1lgilloI1glO1IIIoo9
#define ThreadNewThreadLocalKeyPtr                              ggo0iL9ioggIIOl1ogI1LOOLoOo0og0O
#define ThreadFreeThreadLocalKeyPtr                             Lii1O0olo9991lO1OgLoLlioOgol1io9
#define ThreadGetThreadLocalValue                               o9L0l9lLli9l9g1i9L11gog919LOLgg1
#define ThreadSetThreadLocalValue                               oIlO1oO0iOLL00LigiO9Io0Ig1LLIl1L
#define ThreadGetCurrentThreadID                                lIL9L0LilL1io01OlgIoo9LLL0i9g01I
#define BIT32SET                                                gIIgOglLI9000OOgogllOlil09L1ig0L
#define BIT32SETMASK                                            Oi1L1ioo01LLg0IgLlLL0I1099i9OlO1
#define BIT32MASKSET                                            L1I0iLIg1L9lL909O9OOiO99o9ii0OOo
#define BIT32GET                                                ilgOIL0010ilo9goo0loOI99IlIO011g
#define getInstType                                             IILggolI9l9lio1loiOlLOgiLOOIOoLg
#define initializeARM64InstructionX                             ioii0Lgl1iiI9i01OL00191l0Lg90i0I
#define get_insn_sub                                            lOIgOIIOL1LlilloIioiO9loiOOooL0g
#define insn_equal                                              i0gg1o1l0glog91Io9L1I0Ll9olII0ol
#define arm64_reader_new                                        iOi90lo0L09lii91lg9Ii1lgggLIilgi
#define arm64_reader_init                                       lll0990IL91LII0OI9000l9IiLI9OOlg
#define arm64_reader_reset                                      LO0iOl1IO1lLi1LI0iIIoIO9I19Ili90
#define arm64_reader_free                                       IIooI1liioIgLl9O991ilLlg9lO91o9i
#define arm64_reader_read_one_instruction                       gl0oOlLi9IiolO9oIOlg111glI9lO19o
#define arm64_register_describe                                 oo91OOILog1gIggiOo1l00o9Igiggg0I
#define arm64_relocator_init                                    gloOgI9OL1LLioilOL1LgoLL0gOiog0O
#define arm64_relocator_free                                    Ogi0oigo0g1Ili0l0i1L11ilOOiOO1l0
#define arm64_relocator_reset                                   lo1i0oiLOL09Ol10lig1oiLlIo0LooI1
#define arm64_relocator_read_one                                i11190Llogoi9gIO9L0iIO0ggLgiO1Li
#define arm64_relocator_try_relocate                            i1gl10Ii1iLl1OIl11gI91I00lgILgOg
#define arm64_relocator_relocate_writer                         ggO9I0O00illg1Lg0iioo0oLg1L9g9go
#define arm64_relocator_write_all                               LilIILg19I1Lg1ioOOIIi0oglO0L119g
#define arm64_relocator_write_one                               L10g9gOL0o0o1iI0goOoIOlloolL01iO
#define arm64_relocator_double_write                            IILlL1O9OOoio0oOLgOoIglLI999LlI9
#define arm64_writer_new                                        lO9O1oi1iLI0Li9oggg01Ooo9lgL9Lg9
#define arm64_writer_init                                       OloOiLl0OLggO9OOi100Io90Lo9LllIL
#define arm64_writer_reset                                      LoL99iiI91Lg1g9ig1giIOlLl9lg1goI
#define arm64_writer_free                                       oIl0Og9L1L9gIiLg1Ii9L9iioL19IO9i
#define arm64_writer_put_ldr_br_reg_address                     lgIiLOOioL0glgiLoLg90i0Ooigig9Li
#define arm64_writer_put_ldr_reg_imm                            lio91ggIgoogLIIg1LLgoOLLi01011ol
#define arm64_writer_put_br_reg                                 Li1I9l0Ll10IoIO01g1IogIoOgOloi0I
#define arm64_writer_put_bytes                                  OLligooI0ol90I0OiiL0OlLO09lol0L1
#define arm64_writer_put_ldr_blr_b_reg_address                  oLOI90gIgOLIggOg9loL00lglOLiiLlo
#define arm64_writer_put_blr_reg                                OiIoI1oIl0ii0LlolOI99L999l0Lggio
#define arm64_writer_put_b_imm                                  LOL91l91iI1io1L1oi0g0l1lILL9IgL9
#define arm64_writer_put_ldr_b_reg_address                      ogg9O1L99Il9l9lLLOLO1o1gLo0l0og1
#define arm64_writer_near_jump_range_size                       gig91gOILglol1919ggIo0L00OlOO0il
#define arm64_writer_put_ldr_br_b_reg_address                   gg0i9lO9Lloo9o9g1OLIOlLg9g1iogIl
#define arm64_writer_put_instruction                            L1ol0ig9o9iL90oioOo0igO9lglgIo9I
#define arm64_writer_put_str_reg_reg_offset                     LI901gO1gOiOL9LiI0Ll1I0L9ILo1I1L
#define arm64_writer_put_ldr_reg_reg_offset                     oiglgl1Lg0OLg019OoLlLII0oi0gi99I
#define arm64_writer_put_b_cond_imm                             l9Lg1oggILIi19g0i9OIO19L19OlLO0g
#define arm64_writer_put_add_reg_reg_imm                        IlioIgiOooOi0OIiO10O19gIg99i09LO
#define arm64_writer_put_sub_reg_reg_imm                        II9i91oi09l1g9iogO0LL0LOo90Ig0gL
#define arm64_code_patch                                        L01i9OiL919ILOI9lgOI9o119oLIi000
#define arm64_relocate_code_patch                               oO0g0lIo9og09090OLoI0i1i0iigLOLO
#define common_bridge_handler                                   i1ggoiIgll1i99iIoliOl1I1I9IIO9I0
#define ClosureBridgeAllocate                                   g1IolOllOll1g9O1O1olOgg9olgIgol9
#define closure_bridge_template                                 g0OI1OI1iI0oi9Iog0LL0g190LOIOl90
#define context_begin_invocation                                gllOologio1lI9Lg1lgIlLiigIoI1olL
#define context_begin_invocation_bridge_handler                 L00OL90ILo9ILO9oiiio00IgigOOlLOI
#define context_end_invocation                                  ol90Oo9LL9Loi0I0LgliIl0o0o1lLl19
#define context_end_invocation_bridge_handler                   OOIL09o0LOlliolOOoLLIlg1Loilg9ig
#define dynamic_binary_instrumentation_invocation               I0OooL9l1oLL900liolio9IIO9LOlIiO
#define dynamic_binary_instrumentationn_bridge_handler          O9Ioi1L009lI99l0g9iI9O9i11Il9LLo
#define context_begin_only_invocation                           lLOg0OOOO9gl9L9O9lOI11I19Oi9Lg19
#define context_begin_only_invocation_bridge_handler            oO1oOg0LogII9o1l0Iio0Ogi1Ll0lgiO
#define InteceptorBackendNew                                    LILooogi19ilO9gOOi90I10l9i0Ogl99
#define TrampolineFree                                          O1ioIl91oO9i19olgillIIO1oI909ooL
#define TrampolinePrepare                                       I1g1Lgi09OLOIi9o1OgLl90iloL1glg1
#define TrampolineBuildForEnterTransfer                         OiiLo0liI0ioIiIi9LiIO0olI9iIgi9l
#define TrampolineBuildForEnter                                 I0Iig0Ll0I0ilLO9gIllI90l11gIOgo9
#define TrampolineBuildForEnterOnly                             gLOL9o01OI1iO0giogIIg9OgglioIiI0
#define TrampolineBuildForDynamicBinaryInstrumentation          Li9OLI90l9oLoioL910o1I11ig019LL1
#define TrampolineBuildForInvoke                                IOOgOol0O09i110llL1IgIg01l9O1Ogo
#define TrampolineBuildForLeave                                 glOI911lOgLgO19OLliooO9illLg9olo
#define TrampolineActivate                                      Ii1Ii00001lgO109iO0gl1gglI09LLgl
#define zz_vm_read_string                                       l1IoOg99iO1oOi01IooL9lio1lo1ggIi
#define zz_vm_search_data                                       I0g0iLlglLLlLg1LI0gL9lLIIigiLoii
#define zz_vm_align_floor                                       gg1il1gg9OIiiIgggl99LO9IO91LgLO0
#define zz_vm_align_ceil                                        ooi9gI91O1oigOOO0l1OlOi11i1Il0oo
#define ZmmReadString                                           oo9g9OoOiO1IolO9Il1iOOi9l0i0OIIi
#define ZmmAllocateMemoryPage                                   LlL9l9Oilg0IL90o909lIo1119O09iI9
#define zz_linux_vm_get_memory_layout_via_pid                   Li90l00iOoLLioioo91iOILI9iI1OILo
#define zz_linux_vm_search_code_cave                            Liiog109olL1Li101OiI11lg9LLii11L
#define zz_posix_vm_check_address_valid_via_msync               IgoLigL9igogl1LLgIIiIi0o1oLIOOLL
#define PointerReadFailedHandler                                II000gOOLlig1IILOL0I9191goO00iIo
#define zz_posix_vm_check_address_valid_via_signal              LoLi19lgl99lgIli11L00OiL9Ligi901
#define zz_posix_vm_get_page_size                               i00199OigL910gIlg10lgo1ggg1Lg9gL
#define zz_posix_vm_protect                                     OgOo19l9lo000I1g1oli0LO9OOL0l91g
#define zz_posix_vm_protect_as_executable                       o9ioO001iLIolIiIglo1igil9Loi1iI1
#define zz_posxi_vm_protect_as_writable                         oI9LgI99Oio9LII910g1OIlI9oO1glo9
#define zz_posix_vm_allocate_pages                              oiIIO19olLLLlOL1oo0gLIiO10oLl100
#define zz_posix_vm_allocate                                    gi9IIiLlgoollOL9olOO1g0iOI1LgolI
#define zz_posix_vm_allocate_near_pages                         oo019gl1Og9IOi9oIiI1il010Ill1LIO
#define zz_posix_vm_search_text_code_cave                       iLiOL0ioig90LLoi9l00Oo91oIOoO0g1
#define zz_posix_vm_patch_code                                  loiIogoiOllLIOLgIi9Olo9lL1oi0111
#define zz_posix_thread_new_thread_local_key_list               I9ilOlOIIoO1oOOOlggol1Lo0ool1i1g
#define zz_posix_thread_add_thread_local_key                    lL0L9LiI1glIog90lIoI1gi1Ili0I1Ig
#define zz_posix_thread_free_thread_local_key                   O0ogOiiLOLlIioggOgliO1Lo00gogOI1
#define zz_posix_thread_initialize_thread_local_key_list        lIL00goO0L0OI0009o91lg9goLgolIiO
#define zz_posix_thread_new_thread_local_key_ptr                g0iIOgOlg9gI9L10l9i1ooL9go090I0i
#define zz_posix_thread_get_current_thread_data                 IgIoLI0iIg9Ill1O9iO199ooO1LoOi10
#define zz_posix_thread_set_current_thread_data                 I0991Oil1gI01l1loI0li0l9Ogi9IlI9
#define zz_posix_get_current_thread_id                          O11Ii09Oi0o0IIOoii1LOoIIIg99lO0g

#endif // dx_hook_prefix_h
