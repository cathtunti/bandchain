package db

import (
	"encoding/hex"
	"strings"

	sdk "github.com/cosmos/cosmos-sdk/types"
	"github.com/tendermint/tendermint/crypto"
	"github.com/tendermint/tendermint/libs/common"
)

func (b *BandDB) AddValidator(
	operatorAddress sdk.ValAddress,
	consensusAddress crypto.PubKey,
) error {
	return b.tx.Create(&Validator{
		OperatorAddress:  operatorAddress.String(),
		ConsensusAddress: consensusAddress.Address().String(),
	}).Error
}

func (b *BandDB) AddValidatorUpTime(
	rawConsensusAddress common.HexBytes,
	height int64,
	voted bool,
) error {
	consensusAddress := strings.ToUpper(hex.EncodeToString(rawConsensusAddress))
	err := b.tx.Create(&ValidatorVote{
		ConsensusAddress: consensusAddress,
		BlockHeight:      height,
		Voted:            voted,
	}).Error

	if err != nil {
		return err
	}

	var validator Validator
	err = b.tx.Where(Validator{ConsensusAddress: consensusAddress}).First(&validator).Error
	if err != nil {
		return err
	}

	validator.ElectedCount++
	if voted {
		validator.VotedCount++
	} else {
		validator.MissedCount++
	}

	b.tx.Save(&validator)
	return nil
}

func (b *BandDB) ClearOldVotes(currentHeight int64) error {
	uptimeLookBackDuration, err := b.GetUptimeLookBackDuration()
	if err != nil {
		return err
	}

	if currentHeight > uptimeLookBackDuration {
		var votes []ValidatorVote
		err := b.tx.Find(
			&votes,
			"block_height <= ?",
			currentHeight-uptimeLookBackDuration,
		).Error

		if err != nil {
			return err
		}
		for _, vote := range votes {
			var validator Validator
			err = b.tx.Where(Validator{ConsensusAddress: vote.ConsensusAddress}).First(&validator).Error
			if err == nil {
				validator.ElectedCount--
				if vote.Voted {
					validator.VotedCount--
				} else {
					validator.MissedCount--
				}
				b.tx.Save(&validator)
			}

		}
		return b.tx.Delete(
			ValidatorVote{},
			"block_height <= ?",
			currentHeight-uptimeLookBackDuration,
		).Error
	}
	return nil
}