let component = ReasonReact.statelessComponent("TrelloContainer");

module GetNotifications = [%graphql
  {|
  query trelloNotifications {
    trelloNotifications {
      date
      id
      unread
      type_
      creator {
        fullName
      }
      data {
        attachment {
          attachmentType
          url
        }
        board {
          name
        }
        card {
          closed
          name
          shortLink
        }
        listAfter {
          name
        }
        listBefore {
          name
        }
        text
      }
    }
  }
|}
];

module GetNotificationsQuery = ReasonApollo.CreateQuery(GetNotifications);

module MarkAllAsRead = [%graphql
  {|
    mutation trelloMarkAllAsRead {
      trelloMarkAllAsRead
    }
|}
];

module MarkAllAsReadMutation = ReasonApollo.CreateMutation(MarkAllAsRead);

let make = _children => {
  ...component,

  render: _self =>
    <MarkAllAsReadMutation>
      ...{
           (markAllAsRead, _) =>
             <div className="w-100 w-50-l">
               <GetNotificationsQuery pollInterval=Trello.Config.interval>
                 ...{
                      ({result}) =>
                        <>
                          <Header
                            color="b--light-red"
                            hasItems={
                              switch (result) {
                              | Data(response) =>
                                Belt.Array.length(
                                  response##trelloNotifications,
                                )
                                > 0
                              | _ => false
                              }
                            }
                            markAllAsRead={
                              _ =>
                                markAllAsRead(
                                  ~refetchQueries=[|"trelloNotifications"|],
                                  (),
                                )
                                |> ignore
                            }
                            title="Trello"
                          />
                          <Card>
                            {
                              Trello.Config.hasConfig ?
                                switch (result) {
                                | Loading => <CardLoading />
                                | Error(error) =>
                                  <CardError errorMessage=error##message />
                                | Data(response) =>
                                  let notifications =
                                    response##trelloNotifications;
                                  let groupedByDate =
                                    Utils.groupByDate(notifications);
                                  let hasNotifications =
                                    Belt.Array.length(notifications) > 0;

                                  DocumentTitle.updateTitleWithNotifications()
                                  |> ignore;

                                  Trello.Config.setNumberOfNotifications(
                                    notifications,
                                  );

                                  Notify.sendTrelloNotification(
                                    notifications,
                                  );

                                  hasNotifications ?
                                    Js.Dict.values(groupedByDate)
                                    ->Belt.Array.mapWithIndex((index, list) => {
                                        let title = Js.Dict.keys(
                                                      groupedByDate,
                                                    )[index];

                                        <ListByDate
                                          index
                                          key=title
                                          title
                                          totalItems={
                                            Array.length(
                                              Js.Dict.values(groupedByDate),
                                            )
                                          }>
                                          {
                                            list->Belt.Array.mapWithIndex(
                                              (i, item) =>
                                              <TrelloNotification
                                                item
                                                isLast={
                                                  Belt.Array.length(list)
                                                  - 1 === i
                                                }
                                                key=item##id
                                              />
                                            )
                                            |> ReasonReact.array
                                          }
                                        </ListByDate>;
                                      })
                                    |> ReasonReact.array :
                                    <EmptyState />;
                                } :
                                <TrelloMissing />
                            }
                          </Card>
                        </>
                    }
               </GetNotificationsQuery>
             </div>
         }
    </MarkAllAsReadMutation>,
};
