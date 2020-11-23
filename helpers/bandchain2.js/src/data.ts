import { Address } from 'wallet'

export type HexBytes = Buffer
export type EpochTime = number

export class Coin {
  amount: number
  denom: string

  constructor(amount: number, denom: string) {
    this.amount = amount
    this.denom = denom
  }

  asJson() {
    return { amount: this.amount.toString(), denom: this.denom }
  }
}

export interface Account {
  address: Address
  coins: Coin[]
  publicKey?: object
  accountNumber: number
  sequence: number
}

export interface DataSource {
  owner: Address
  name: string
  description: string
  fileName: string
}

export interface ReferenceDataUpdated {
  base: number
  quote: number
}

export interface ReferenceData {
  pair: string
  rate: number
  updatedAt: ReferenceDataUpdated
}

export interface TransactionSyncMode {
  txHash: HexBytes
  code: number
  errorLog?: string
}

export interface TransactionAsyncMode {
  txHash: HexBytes
}

export interface TransactionBlockMode {
  height: number
  txHash: HexBytes
  gasWanted: number
  gasUsed: number
  code: number
  log: object[]
  errorLog?: string
}

export interface BlockHeaderInfo {
  chainID: string
  height: number
  time: EpochTime
  lastCommitHash: HexBytes
  dataHash: HexBytes
  validatorsHash: HexBytes
  nextValidatorsHash: HexBytes
  consensusHash: HexBytes
  appHash: HexBytes
  lastResultsHash: HexBytes
  evidenceHash: HexBytes
  proposerAddress: HexBytes
}
export interface BlockHeader {
  header: BlockHeaderInfo
}

export interface BlockID {
  hash: HexBytes
}
export interface Block {
  block: BlockHeader
  blockID: BlockID
}
