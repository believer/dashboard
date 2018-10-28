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

let handleNotifications = response => {
  let notifications = response##trelloNotifications;
  let groupedByDate = Utils.groupByDate(notifications);
  let hasNotifications = Belt.Array.length(notifications) > 0;

  DocumentTitle.updateTitleWithNotifications() |> ignore;

  Trello.Config.setNumberOfNotifications(notifications);

  Notify.sendTrelloNotification(notifications);

  hasNotifications ?
    Js.Dict.values(groupedByDate)
    ->Belt.Array.mapWithIndex((index, list) => {
        let title = Js.Dict.keys(groupedByDate)[index];

        <ListByDate
          index
          key=title
          title
          totalItems={Array.length(Js.Dict.values(groupedByDate))}>
          {
            list->Belt.Array.mapWithIndex((i, item) =>
              <TrelloNotification
                item
                isLast={Belt.Array.length(list) - 1 === i}
                key=item##id
              />
            )
            |> ReasonReact.array
          }
        </ListByDate>;
      })
    |> ReasonReact.array :
    <EmptyState />;
};

let make = _children => {
  ...component,

  render: _self => {
    let%Epitath {result} =
      children =>
        <GetNotificationsQuery pollInterval=Trello.Config.interval>
          ...children
        </GetNotificationsQuery>;

    let hasItems =
      switch (result) {
      | Data(response) =>
        Belt.Array.length(response##trelloNotifications) > 0
      | _ => false
      };

    <MarkAllAsReadMutation>
      ...{
           (markAllAsRead, _) =>
             <div className="w-100 w-50-l">
               <Header
                 color="b--light-red"
                 hasItems
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
                 <Notifications
                   hasConfig=Trello.Config.hasConfig
                   missing={<TrelloMissing />}
                   result
                   render=handleNotifications
                 />
               </Card>
             </div>
         }
    </MarkAllAsReadMutation>;
  },
};
