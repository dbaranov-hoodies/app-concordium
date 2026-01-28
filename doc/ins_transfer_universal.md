# Transfer transaction

A transaction to transfer GTU from one account to another. Combines functionality of sign transfer, sign transfer with memo, sign transfer with schedule

## Protocol description

- Command sequence
HEADER -> MEMO(optional, multiple) -> SCHEDULE(optional, multiple) -> AMOUNT -> FEES -> FINAL


| INS    | P1     | P2     | CDATA                                                                                                                                  | Comment                                                        |
| ------ | ------ | ------ | -------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- |
| `0x42` | `0x00` | `0x00..0x03` | `path_length path[uint32]x[8] account_transaction_header[60 bytes] transaction_kind[uint8] recipient_address[32 bytes] scheduled_amounts_count[uint8]? memo_length[uint16]?` | Transaction header |
| `0x42` | `0x20` | `0x00..0x03` | `chunk of memo[1...255 bytes]` | Transmit memo chunks |
| `0x42` | `0x30` | `0x00..0x03` | `chunk of schedule` | Transmit schedule chunks (TBD) |
| `0x42` | `0x40` | `0x00..0x03` | `amount[uint64]` | Transmit tx amount  |
| `0x42` | `0x41` | `0x00..0x03` | `fees[uint64]` | Transmit tx fees |
| `0x42` | `0x80` | `0x00..0x03` | `empty` | finalizes transmission and triggers device display activity |

P2 is a bit field which defines presence of schedule and memo
0x00 -- no memo,no schedule, 
0x01 -- has memo, no schedule
0x02 -- no memo, has schedule
0x03 -- has memo, has schedule

Optional fields in transaction header: 
scheduled_amounts_count is present when tx has schedule, i.e. p2 is 0x02 or 0x03
memo_length is present when tx has memo, i.e. p2 is 0x01 or 0x03


