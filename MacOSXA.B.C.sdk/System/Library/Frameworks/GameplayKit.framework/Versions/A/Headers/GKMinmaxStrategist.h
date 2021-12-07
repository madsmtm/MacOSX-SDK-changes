//
//  GKMinmaxStrategist.h
//  GameLogic
//
//  Copyright (c) 2014 Apple. All rights reserved.
//

#import <GameplayKit/GKGameModel.h>
#import <GameplayKit/GKRandomSource.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * The Minmax Strategist is a generic AI that selects a game model update for a given player that maximises 
 * potential gain, while minimising potential loss. It does this by examining all of the updates available 
 * to the player in question, extrapolating the potential moves opposing players may take, projecting out 
 * maxLookAheadDepth number of turns. The selected update will result in the greatest potential gain, balanced 
 * against the potential gain of other players.
 */
GK_BASE_AVAILABILITY @interface GKMinmaxStrategist : NSObject

/**
 * The game model that we wish to select updates for.
 */
@property (nonatomic, retain, nullable) id<GKGameModel> gameModel;

/**
 * The maximum number of future turns that will be processed when searching for a move.
 */
@property (nonatomic, assign) NSInteger maxLookAheadDepth;

/**
 * A random source to use when breaking ties between equally-strong moves when calling bestMoveForPlayer 
 * or when selecting a random move when randomMoveForPlayer is called. If set to nil, bestMoveForPlayer 
 * and randomMoveForPlayer will simply return the first best move available.
 */
@property (nonatomic, retain, nullable) id<GKRandom> randomSource;

/**
 * Selects the best move for the specified player. If randomSource is not nil, it will randomly select 
 * which move to use if there are one or more ties for the best. Returns nil if the player is invalid, 
 * the player is not a part of the game model, or the player has no valid moves available.
 */
- (nullable id<GKGameModelUpdate>)bestMoveForPlayer:(id<GKGameModelPlayer>)player;

/**
 * Selects one move from the set of N best moves for the specified player, where N is equal to 
 * numMovesToConsider. If randomSource is nil, it will not randomly select, but will behave like 
 * bestMoveForPlayer and return the first best move. Returns nil if the player is invalid, the 
 * player is not a part of the game model, or the player has no valid moves available.
 */
- (nullable id<GKGameModelUpdate>)randomMoveForPlayer:(id<GKGameModelPlayer>)player fromNumberOfBestMoves:(NSInteger)numMovesToConsider;

@end

NS_ASSUME_NONNULL_END
